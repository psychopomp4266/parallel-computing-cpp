#include <iostream>
#include <vector>
#include <chrono>
#include <algorithm>

int main()
{
    // ============================================================
    // Experiment 1 — Tiny AXPY correctness test
    // Purpose: Verify the basic AXPY calculation with a small N.
    // ============================================================

    // const int N = 10;


    // ============================================================
    // Experiment 2 — Large AXPY workload
    // Purpose: Use a sufficiently large dataset for benchmarking.
    // ============================================================

    const int N = 10'000'000;
    const float alpha = 2.0f;

    std::vector<float> x(N);
    std::vector<float> y(N);


    // ============================================================
    // Common setup — Initialize input vectors
    // ============================================================

    for (int i = 0; i < N; ++i)
    {
        x[i] = static_cast<float>(i);
        y[i] = 100.0f + static_cast<float>(i);
    }


    // ============================================================
    // Experiment 1 — Tiny AXPY correctness test
    // Purpose: Perform the basic serial AXPY operation.
    // ============================================================

    /*
    for (int i = 0; i < N; ++i)
    {
        y[i] = alpha * x[i] + y[i];
    }
    */


    // ============================================================
    // Experiment 1 — Print every result
    // Purpose: Inspect every element for a tiny N.
    // ============================================================

    /*
    for (int i = 0; i < N; ++i)
    {
        std::cout << "y[" << i << "] = " << y[i] << '\n';
    }
    */


    // ============================================================
    // Experiment 2 — Print first 10 results
    // Purpose: Verify correctness without printing millions
    //          of elements.
    // ============================================================

    /*
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "y[" << i << "] = " << y[i] << '\n';
    }
    */


    // ============================================================
    // Experiment 3 — Single-run AXPY timing
    // Purpose: Measure the execution time of one serial AXPY pass.
    // ============================================================

    /*
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < N; ++i)
    {
        y[i] = alpha * x[i] + y[i];
    }

    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    std::cout << "AXPY time: "
              << elapsed.count()
              << " seconds\n";
    */


    // ============================================================
    // Experiment 4 — Repeated serial AXPY timing
    // Purpose: Measure multiple equivalent AXPY runs so that we
    //          can observe timing variation.
    // ============================================================

    /*
    const int num_runs = 10;

    for (int run = 0; run < num_runs; ++run)
    {
        // Reset y to the same initial values before each run.
        // This reset is intentionally outside the timed region.
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        auto start = std::chrono::steady_clock::now();

        // Serial AXPY:
        // y[i] = alpha * x[i] + y[i]
        for (int i = 0; i < N; ++i)
        {
            y[i] = alpha * x[i] + y[i];
        }

        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed = end - start;

        std::cout << "Run " << run + 1
                  << ": " << elapsed.count()
                  << " seconds\n";
    }
    */


    // ============================================================
    // Experiment 5 — Warm-up + repeated serial timing
    // Purpose: Run a few untimed AXPY passes before collecting
    //          measured timings, reducing startup/transient effects.
    // ============================================================

    /*
    const int num_warmup_runs = 2;

    // ------------------------------------------------------------
    // Warm-up phase — intentionally NOT timed
    // ------------------------------------------------------------

    for (int run = 0; run < num_warmup_runs; ++run)
    {
        // Reset y to the same initial values.
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        // Perform the same serial AXPY computation.
        for (int i = 0; i < N; ++i)
        {
            y[i] = alpha * x[i] + y[i];
        }
    }


    // ------------------------------------------------------------
    // Measured phase
    // ------------------------------------------------------------

    const int num_runs = 10;

    for (int run = 0; run < num_runs; ++run)
    {
        // Reset y to the same initial values before each run.
        // This reset is intentionally outside the timed region.
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        auto start = std::chrono::steady_clock::now();

        // Serial AXPY:
        // y[i] = alpha * x[i] + y[i]
        for (int i = 0; i < N; ++i)
        {
            y[i] = alpha * x[i] + y[i];
        }

        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed = end - start;

        std::cout << "Run " << run + 1
                  << ": " << elapsed.count()
                  << " seconds\n";
    }
    */


    // ============================================================
    // Experiment 6 — Timing statistics + effective bandwidth
    // Purpose: Collect repeated timings and summarize them using
    //          minimum, maximum, mean, median, and bandwidth.
    // ============================================================

    /*
    const int num_warmup_runs = 2;

    // ------------------------------------------------------------
    // Warm-up phase — intentionally NOT timed
    // ------------------------------------------------------------

    for (int run = 0; run < num_warmup_runs; ++run)
    {
        // Reset y to the same initial values.
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        // Perform the same serial AXPY computation.
        for (int i = 0; i < N; ++i)
        {
            y[i] = alpha * x[i] + y[i];
        }
    }


    // ------------------------------------------------------------
    // Measured phase
    // ------------------------------------------------------------

    const int num_runs = 10;
    double timings[num_runs];

    for (int run = 0; run < num_runs; ++run)
    {
        // Reset y to the same initial values before each run.
        // This reset is intentionally outside the timed region.
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        auto start = std::chrono::steady_clock::now();

        // Serial AXPY:
        // y[i] = alpha * x[i] + y[i]
        for (int i = 0; i < N; ++i)
        {
            y[i] = alpha * x[i] + y[i];
        }

        auto end = std::chrono::steady_clock::now();

        std::chrono::duration<double> elapsed = end - start;
        timings[run] = elapsed.count();

        std::cout << "Run " << run + 1
                  << ": " << elapsed.count()
                  << " seconds\n";
    }

    double min_time = timings[0];
    double max_time = timings[0];
    double sum = 0.0;

    for (int i = 0; i < num_runs; ++i)
    {
        if (timings[i] < min_time)
            min_time = timings[i];

        if (timings[i] > max_time)
            max_time = timings[i];

        sum += timings[i];
    }

    double mean_time = sum / num_runs;

    // Make a copy so that the original timing order is preserved.
    double sorted_timings[num_runs];

    for (int i = 0; i < num_runs; ++i)
    {
        sorted_timings[i] = timings[i];
    }

    std::sort(sorted_timings, sorted_timings + num_runs);

    double median_time;

    if (num_runs % 2 == 0)
    {
        median_time = (sorted_timings[num_runs / 2 - 1]
                    + sorted_timings[num_runs / 2]) / 2.0;
    }
    else
    {
        median_time = sorted_timings[num_runs / 2];
    }

    // ------------------------------------------------------------
    // Effective memory bandwidth
    // ------------------------------------------------------------

    // Each float AXPY element approximately involves:
    //   4 bytes  -> load x[i]
    //   4 bytes  -> load y[i]
    //   4 bytes  -> store y[i]
    // Total = 12 bytes per element.

    const double bytes_per_element = 12.0;
    const double total_bytes = bytes_per_element * N;

    // Convert bytes to gigabytes using decimal GB.
    const double total_gigabytes = total_bytes / 1e9;

    double bandwidth_min = total_gigabytes / min_time;
    double bandwidth_mean = total_gigabytes / mean_time;
    double bandwidth_median = total_gigabytes / median_time;

    std::cout << "\nTiming statistics:\n";
    std::cout << "Minimum: " << min_time << " seconds\n";
    std::cout << "Maximum: " << max_time << " seconds\n";
    std::cout << "Mean:    " << mean_time << " seconds\n";
    std::cout << "Median:  " << median_time << " seconds\n";

    std::cout << "\nEffective memory bandwidth:\n";
    std::cout << "Using minimum time: "
              << bandwidth_min
              << " GB/s\n";

    std::cout << "Using mean time:    "
              << bandwidth_mean
              << " GB/s\n";

    std::cout << "Using median time:  "
              << bandwidth_median
              << " GB/s\n";
    */


    // ============================================================
    // Experiment 7 — Statistical outlier analysis
    // Purpose: Use a predefined IQR-based rule to identify
    //          unusually slow or fast timing samples and compare
    //          raw statistics with statistics from the filtered set.
    // ============================================================

    const int num_warmup_runs = 2;

    // ------------------------------------------------------------
    // Warm-up phase — intentionally NOT timed
    // ------------------------------------------------------------

    for (int run = 0; run < num_warmup_runs; ++run)
    {
        // Reset y to the same initial values.
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        // Perform the same serial AXPY computation.
        for (int i = 0; i < N; ++i)
        {
            y[i] = alpha * x[i] + y[i];
        }
    }


    // ------------------------------------------------------------
    // Measured phase — collect all raw timings
    // ------------------------------------------------------------

    // const int num_runs = 10;


    // For Exp 8 take it to 50
    const int num_runs = 50;


    std::vector<double> timings(num_runs);

    for (int run = 0; run < num_runs; ++run)
    {
        // Reset y to the same initial values before each run.
        // This reset is intentionally outside the timed region.
        for (int i = 0; i < N; ++i)
        {
            y[i] = 100.0f + static_cast<float>(i);
        }

        auto start = std::chrono::steady_clock::now();

        // Serial AXPY:
        // y[i] = alpha * x[i] + y[i]
        
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


    // ------------------------------------------------------------
    // Raw timing statistics
    // ------------------------------------------------------------

    std::vector<double> sorted_timings = timings;

    std::sort(sorted_timings.begin(), sorted_timings.end());

    double min_time = sorted_timings.front();
    double max_time = sorted_timings.back();

    double sum = 0.0;

    for (double time : timings)
    {
        sum += time;
    }

    double mean_time = sum / num_runs;

    double median_time;

    if (num_runs % 2 == 0)
    {
        median_time = (sorted_timings[num_runs / 2 - 1]
                     + sorted_timings[num_runs / 2]) / 2.0;
    }
    else
    {
        median_time = sorted_timings[num_runs / 2];
    }


    // ------------------------------------------------------------
    // IQR calculation
    // ------------------------------------------------------------

    // For an even number of samples, divide the sorted data into
    // a lower half and an upper half.
    //
    // Q1 = median of the lower half
    // Q3 = median of the upper half
    //
    // IQR = Q3 - Q1

    const int half_size = num_runs / 2;

    double q1 = (sorted_timings[half_size / 2 - 1]
               + sorted_timings[half_size / 2]) / 2.0;

    double q3 = (sorted_timings[half_size + half_size / 2 - 1]
               + sorted_timings[half_size + half_size / 2]) / 2.0;

    double iqr = q3 - q1;


    // ------------------------------------------------------------
    // IQR-based outlier boundaries
    // ------------------------------------------------------------

    // Standard Tukey rule:
    //
    // Lower boundary = Q1 - 1.5 * IQR
    // Upper boundary = Q3 + 1.5 * IQR

    const double iqr_multiplier = 1.5;

    double lower_bound = q1 - iqr_multiplier * iqr;
    double upper_bound = q3 + iqr_multiplier * iqr;


    // ------------------------------------------------------------
    // Identify and remove statistical outliers
    // ------------------------------------------------------------

    std::vector<double> filtered_timings;

    std::cout << "\nOutlier analysis:\n";
    std::cout << "Q1: " << q1 << " seconds\n";
    std::cout << "Q3: " << q3 << " seconds\n";
    std::cout << "IQR: " << iqr << " seconds\n";
    std::cout << "Lower boundary: "
              << lower_bound
              << " seconds\n";
    std::cout << "Upper boundary: "
              << upper_bound
              << " seconds\n";

    for (int i = 0; i < num_runs; ++i)
    {
        if (timings[i] < lower_bound || timings[i] > upper_bound)
        {
            std::cout << "Run " << i + 1
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
    // Filtered timing statistics
    // ------------------------------------------------------------

    std::sort(filtered_timings.begin(), filtered_timings.end());

    double filtered_min = filtered_timings.front();
    double filtered_max = filtered_timings.back();

    double filtered_sum = 0.0;

    for (double time : filtered_timings)
    {
        filtered_sum += time;
    }

    double filtered_mean =
        filtered_sum / filtered_timings.size();

    double filtered_median;

    int filtered_count = static_cast<int>(filtered_timings.size());

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


    // ------------------------------------------------------------
    // Effective memory bandwidth
    // ------------------------------------------------------------

    // Each float AXPY element approximately involves:
    //   4 bytes  -> load x[i]
    //   4 bytes  -> load y[i]
    //   4 bytes  -> store y[i]
    //
    // Total = 12 bytes per element.

    const double bytes_per_element = 12.0;
    const double total_bytes = bytes_per_element * N;

    // Convert bytes to decimal gigabytes.
    const double total_gigabytes = total_bytes / 1e9;

    double bandwidth_min =
        total_gigabytes / min_time;

    double bandwidth_mean =
        total_gigabytes / mean_time;

    double bandwidth_median =
        total_gigabytes / median_time;

    double filtered_bandwidth_min =
        total_gigabytes / filtered_min;

    double filtered_bandwidth_mean =
        total_gigabytes / filtered_mean;

    double filtered_bandwidth_median =
        total_gigabytes / filtered_median;


    // ------------------------------------------------------------
    // Final results
    // ------------------------------------------------------------

    std::cout << "\nRaw timing statistics:\n";
    std::cout << "Minimum: " << min_time << " seconds\n";
    std::cout << "Maximum: " << max_time << " seconds\n";
    std::cout << "Mean:    " << mean_time << " seconds\n";
    std::cout << "Median:  " << median_time << " seconds\n";

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
              << filtered_timings.size()
              << " / " << num_runs << "\n";

    std::cout << "Samples removed:  "
              << num_runs - filtered_count
              << " / " << num_runs << "\n";

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


    return 0;
}