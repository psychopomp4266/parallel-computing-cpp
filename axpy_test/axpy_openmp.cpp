#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <numeric>
#include <omp.h>

int main()
{
    const int N = 10'000'000;
    const float alpha = 2.0f;

    // ============================================================
    // Experiment 3: Repeated OpenMP benchmark
    // ============================================================

    const int num_warmup_runs = 2;
    const int num_runs = 50;

    std::vector<float> x(N), y(N);

    // ------------------------------------------------------------
    // Initialization
    // ------------------------------------------------------------

    for (int i = 0; i < N; ++i)
    {
        x[i] = static_cast<float>(i);
        y[i] = 100.0f + static_cast<float>(i);
    }


    // for (int requested = 1; requested <= 8; requested *= 2)
    // {
    //     omp_set_num_threads(requested);

    //     #pragma omp parallel
    //     {
    //         #pragma omp single
    //         {
    //             std::cout << "Requested: " << requested
    //                     << " | Actual team size: "
    //                     << omp_get_num_threads()
    //                     << '\n';
    //         }
    //     }
    // }

    // ------------------------------------------------------------
    // Warm-up runs
    // ------------------------------------------------------------

    for (int warmup = 0; warmup < num_warmup_runs; ++warmup)
    {
        // Reset y outside the AXPY loop
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        #pragma omp parallel for
        for (int i = 0; i < N; ++i)
        {
            y[i] = alpha * x[i] + y[i];
        }
    }

    // ------------------------------------------------------------
    // Measured runs
    // ------------------------------------------------------------

    std::vector<double> timings(num_runs);

    for (int run = 0; run < num_runs; ++run)
    {
        // Reset y outside the timed region
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        auto start = std::chrono::steady_clock::now();

        #pragma omp parallel for
        for (int i = 0; i < N; ++i)
        {
            y[i] = alpha * x[i] + y[i];
        }

        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed = end - start;

        timings[run] = elapsed.count();

        std::cout << "Run " << run + 1
                  << ": " << timings[run]
                  << " seconds\n";
    }

    // ============================================================
    // Statistical analysis
    // ============================================================

    // ------------------------------------------------------------
    // Raw timing statistics
    // ------------------------------------------------------------

    double min_time = *std::min_element(
        timings.begin(), timings.end());

    double max_time = *std::max_element(
        timings.begin(), timings.end());

    double mean_time =
        std::accumulate(
            timings.begin(),
            timings.end(),
            0.0) / num_runs;

    std::vector<double> sorted_timings = timings;

    std::sort(
        sorted_timings.begin(),
        sorted_timings.end());

    double median_time;

    if (num_runs % 2 == 0)
    {
        median_time =
            (sorted_timings[num_runs / 2 - 1]
             + sorted_timings[num_runs / 2]) / 2.0;
    }
    else
    {
        median_time =
            sorted_timings[num_runs / 2];
    }

    // ------------------------------------------------------------
    // Quartiles using median-of-halves
    // ------------------------------------------------------------

    const int half_size = num_runs / 2;

    double Q1;
    double Q3;

    if (half_size % 2 == 0)
    {
        Q1 =
            (sorted_timings[half_size / 2 - 1]
             + sorted_timings[half_size / 2]) / 2.0;

        Q3 =
            (sorted_timings[
                half_size + half_size / 2 - 1]
             + sorted_timings[
                half_size + half_size / 2]) / 2.0;
    }
    else
    {
        Q1 = sorted_timings[half_size / 2];

        Q3 =
            sorted_timings[
                half_size + half_size / 2];
    }

    double IQR = Q3 - Q1;

    // ------------------------------------------------------------
    // Tukey 1.5 × IQR outlier boundaries
    // ------------------------------------------------------------

    double lower_bound = Q1 - 1.5 * IQR;
    double upper_bound = Q3 + 1.5 * IQR;

    // ------------------------------------------------------------
    // Identify outliers and construct filtered dataset
    // ------------------------------------------------------------

    std::vector<double> filtered_timings;

    for (int i = 0; i < num_runs; ++i)
    {
        if (timings[i] < lower_bound ||
            timings[i] > upper_bound)
        {
            std::cout
                << "Run " << i + 1
                << " identified as outlier: "
                << timings[i]
                << " seconds\n";
        }
        else
        {
            filtered_timings.push_back(timings[i]);
        }
    }

    // ------------------------------------------------------------
    // Filtered statistics
    // ------------------------------------------------------------

    double filtered_min =
        *std::min_element(
            filtered_timings.begin(),
            filtered_timings.end());

    double filtered_max =
        *std::max_element(
            filtered_timings.begin(),
            filtered_timings.end());

    double filtered_mean =
        std::accumulate(
            filtered_timings.begin(),
            filtered_timings.end(),
            0.0)
        / filtered_timings.size();

    std::sort(
        filtered_timings.begin(),
        filtered_timings.end());

    double filtered_median;

    int filtered_count =
        static_cast<int>(filtered_timings.size());

    if (filtered_count % 2 == 0)
    {
        filtered_median =
            (filtered_timings[filtered_count / 2 - 1]
             + filtered_timings[filtered_count / 2]) / 2.0;
    }
    else
    {
        filtered_median =
            filtered_timings[filtered_count / 2];
    }

    // ============================================================
    // Effective memory bandwidth
    // ============================================================

    // AXPY:
    //
    // y[i] = alpha * x[i] + y[i]
    //
    // Approximate memory traffic per element:
    //
    // Load x = 4 bytes
    // Load y = 4 bytes
    // Store y = 4 bytes
    //
    // Total = 12 bytes / element

    const double bytes_per_element = 12.0;

    const double total_bytes =
        bytes_per_element * static_cast<double>(N);

    const double total_GB =
        total_bytes / 1e9;

    double bandwidth_min =
        total_GB / min_time;

    double bandwidth_mean =
        total_GB / mean_time;

    double bandwidth_median =
        total_GB / median_time;

    double filtered_bandwidth_min =
        total_GB / filtered_min;

    double filtered_bandwidth_mean =
        total_GB / filtered_mean;

    double filtered_bandwidth_median =
        total_GB / filtered_median;

    // ============================================================
    // Print results
    // ============================================================

    std::cout << "\n";

    std::cout << "OpenMP max threads = "
              << omp_get_max_threads()
              << '\n';

    std::cout << "\nOutlier analysis:\n";

    std::cout << "Q1: "
              << Q1
              << " seconds\n";

    std::cout << "Q3: "
              << Q3
              << " seconds\n";

    std::cout << "IQR: "
              << IQR
              << " seconds\n";

    std::cout << "Lower boundary: "
              << lower_bound
              << " seconds\n";

    std::cout << "Upper boundary: "
              << upper_bound
              << " seconds\n";

    std::cout << "\nRaw timing statistics:\n";

    std::cout << "Minimum: "
              << min_time
              << " seconds\n";

    std::cout << "Maximum: "
              << max_time
              << " seconds\n";

    std::cout << "Mean:    "
              << mean_time
              << " seconds\n";

    std::cout << "Median:  "
              << median_time
              << " seconds\n";

    std::cout << "\nRaw effective memory bandwidth:\n";

    std::cout << "Using minimum time: "
              << bandwidth_min
              << " GB/s\n";

    std::cout << "Using mean time:    "
              << bandwidth_mean
              << " GB/s\n";

    std::cout << "Using median time:  "
              << bandwidth_median
              << " GB/s\n";

    std::cout << "\nFiltered timing statistics:\n";

    std::cout << "Samples retained: "
              << filtered_count
              << " / "
              << num_runs
              << '\n';

    std::cout << "Samples removed: "
              << num_runs - filtered_count
              << " / "
              << num_runs
              << '\n';

    std::cout << "Minimum: "
              << filtered_min
              << " seconds\n";

    std::cout << "Maximum: "
              << filtered_max
              << " seconds\n";

    std::cout << "Mean:    "
              << filtered_mean
              << " seconds\n";

    std::cout << "Median:  "
              << filtered_median
              << " seconds\n";

    std::cout << "\nFiltered effective memory bandwidth:\n";

    std::cout << "Using minimum time: "
              << filtered_bandwidth_min
              << " GB/s\n";

    std::cout << "Using mean time:    "
              << filtered_bandwidth_mean
              << " GB/s\n";

    std::cout << "Using median time:  "
              << filtered_bandwidth_median
              << " GB/s\n";

    // ============================================================
    // Correctness verification
    // ============================================================

    bool correct = true;

    for (int i = 0; i < N; ++i)
    {
        float expected =
            alpha * static_cast<float>(i)
            + (100.0f + static_cast<float>(i));

        if (y[i] != expected)
        {
            correct = false;

            std::cout
                << "\nMismatch at index "
                << i
                << ": expected "
                << expected
                << ", got "
                << y[i]
                << '\n';

            break;
        }
    }

    std::cout << "\nCorrectness: "
              << (correct ? "PASS" : "FAIL")
              << '\n';

    // ------------------------------------------------------------
    // Sample output values
    // ------------------------------------------------------------

    std::cout << "y[0] = " << y[0] << '\n';
    std::cout << "y[1] = " << y[1] << '\n';
    std::cout << "y[2] = " << y[2] << '\n';
    std::cout << "y[3] = " << y[3] << '\n';
    std::cout << "y[4] = " << y[4] << '\n';

    return 0;
}

