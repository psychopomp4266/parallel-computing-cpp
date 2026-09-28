#ifndef BENCHMARK_UTILS_H
#define BENCHMARK_UTILS_H

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <vector>

namespace benchmark
{
    struct Statistics
    {
        double min;
        double max;
        double mean;
        double median;

        double q1;
        double q3;
        double iqr;

        double lower_boundary;
        double upper_boundary;

        std::vector<std::size_t> outlier_indices;
        std::vector<double> filtered_samples;

        double filtered_min;
        double filtered_max;
        double filtered_mean;
        double filtered_median;
    };

    // Calculate the median of a sorted range.
    inline double median_sorted(
        const std::vector<double>& values,
        std::size_t begin,
        std::size_t end)
    {
        const std::size_t count = end - begin;

        if (count == 0)
        {
            return 0.0;
        }

        const std::size_t middle = begin + count / 2;

        if (count % 2 == 0)
        {
            return (values[middle - 1] + values[middle]) / 2.0;
        }

        return values[middle];
    }

    // Calculate Q1 and Q3 using the median-of-halves method.
    inline void calculate_quartiles(
        const std::vector<double>& sorted_values,
        double& q1,
        double& q3)
    {
        const std::size_t n = sorted_values.size();
        const std::size_t middle = n / 2;

        q1 = median_sorted(sorted_values, 0, middle);

        if (n % 2 == 0)
        {
            q3 = median_sorted(sorted_values, middle, n);
        }
        else
        {
            q3 = median_sorted(sorted_values, middle + 1, n);
        }
    }

    inline double calculate_mean(const std::vector<double>& values)
    {
        if (values.empty())
        {
            return 0.0;
        }

        const double sum =
            std::accumulate(values.begin(), values.end(), 0.0);

        return sum / static_cast<double>(values.size());
    }

    inline Statistics analyze(
        const std::vector<double>& samples)
    {
        Statistics result{};

        if (samples.empty())
        {
            return result;
        }

        // ------------------------------------------------------------
        // Raw statistics
        // ------------------------------------------------------------

        result.min = *std::min_element(
            samples.begin(),
            samples.end());

        result.max = *std::max_element(
            samples.begin(),
            samples.end());

        result.mean = calculate_mean(samples);

        std::vector<double> sorted_samples = samples;

        std::sort(
            sorted_samples.begin(),
            sorted_samples.end());

        result.median = median_sorted(
            sorted_samples,
            0,
            sorted_samples.size());

        // ------------------------------------------------------------
        // Quartiles and IQR
        // ------------------------------------------------------------

        calculate_quartiles(
            sorted_samples,
            result.q1,
            result.q3);

        result.iqr = result.q3 - result.q1;

        result.lower_boundary =
            result.q1 - 1.5 * result.iqr;

        result.upper_boundary =
            result.q3 + 1.5 * result.iqr;

        // ------------------------------------------------------------
        // Identify outliers and create filtered sample set
        // ------------------------------------------------------------

        for (std::size_t i = 0; i < samples.size(); ++i)
        {
            const double value = samples[i];

            if (value < result.lower_boundary ||
                value > result.upper_boundary)
            {
                result.outlier_indices.push_back(i);
            }
            else
            {
                result.filtered_samples.push_back(value);
            }
        }

        // ------------------------------------------------------------
        // Filtered statistics
        // ------------------------------------------------------------

        if (!result.filtered_samples.empty())
        {
            result.filtered_min =
                *std::min_element(
                    result.filtered_samples.begin(),
                    result.filtered_samples.end());

            result.filtered_max =
                *std::max_element(
                    result.filtered_samples.begin(),
                    result.filtered_samples.end());

            result.filtered_mean =
                calculate_mean(result.filtered_samples);

            std::vector<double> sorted_filtered =
                result.filtered_samples;

            std::sort(
                sorted_filtered.begin(),
                sorted_filtered.end());

            result.filtered_median =
                median_sorted(
                    sorted_filtered,
                    0,
                    sorted_filtered.size());
        }

        return result;
    }

    inline double calculate_bandwidth(
        double execution_time,
        std::size_t N,
        double bytes_per_element = 12.0)
    {
        if (execution_time <= 0.0)
        {
            return 0.0;
        }

        const double total_bytes =
            static_cast<double>(N) * bytes_per_element;

        return total_bytes /
               execution_time /
               1e9;
    }

    inline void print_outliers(
        const std::vector<double>& samples,
        const Statistics& stats)
    {
        for (std::size_t index : stats.outlier_indices)
        {
            std::cout
                << "Run " << index + 1
                << " identified as outlier: "
                << samples[index]
                << " seconds\n";
        }
    }

    inline void print_report(
        const std::vector<double>& samples,
        const Statistics& stats,
        std::size_t N)
    {
        std::cout << std::fixed << std::setprecision(7);

        // ------------------------------------------------------------
        // Outlier analysis
        // ------------------------------------------------------------

        print_outliers(samples, stats);

        std::cout << "\nOutlier analysis:\n";
        std::cout << "Q1: "
                  << stats.q1
                  << " seconds\n";

        std::cout << "Q3: "
                  << stats.q3
                  << " seconds\n";

        std::cout << "IQR: "
                  << stats.iqr
                  << " seconds\n";

        std::cout << "Lower boundary: "
                  << stats.lower_boundary
                  << " seconds\n";

        std::cout << "Upper boundary: "
                  << stats.upper_boundary
                  << " seconds\n";

        // ------------------------------------------------------------
        // Raw timing statistics
        // ------------------------------------------------------------

        std::cout << "\nRaw timing statistics:\n";

        std::cout << "Minimum: "
                  << stats.min
                  << " seconds\n";

        std::cout << "Maximum: "
                  << stats.max
                  << " seconds\n";

        std::cout << "Mean:    "
                  << stats.mean
                  << " seconds\n";

        std::cout << "Median:  "
                  << stats.median
                  << " seconds\n";

        // ------------------------------------------------------------
        // Raw bandwidth
        // ------------------------------------------------------------

        std::cout << "\nRaw effective memory bandwidth:\n";

        std::cout << "Using minimum time: "
                  << calculate_bandwidth(
                         stats.min, N)
                  << " GB/s\n";

        std::cout << "Using mean time:    "
                  << calculate_bandwidth(
                         stats.mean, N)
                  << " GB/s\n";

        std::cout << "Using median time:  "
                  << calculate_bandwidth(
                         stats.median, N)
                  << " GB/s\n";

        // ------------------------------------------------------------
        // Filtered timing statistics
        // ------------------------------------------------------------

        std::cout << "\nFiltered timing statistics:\n";

        std::cout << "Samples retained: "
                  << stats.filtered_samples.size()
                  << " / "
                  << samples.size()
                  << '\n';

        std::cout << "Samples removed: "
                  << stats.outlier_indices.size()
                  << " / "
                  << samples.size()
                  << '\n';

        std::cout << "Minimum: "
                  << stats.filtered_min
                  << " seconds\n";

        std::cout << "Maximum: "
                  << stats.filtered_max
                  << " seconds\n";

        std::cout << "Mean:    "
                  << stats.filtered_mean
                  << " seconds\n";

        std::cout << "Median:  "
                  << stats.filtered_median
                  << " seconds\n";

        // ------------------------------------------------------------
        // Filtered bandwidth
        // ------------------------------------------------------------

        std::cout << "\nFiltered effective memory bandwidth:\n";

        std::cout << "Using minimum time: "
                  << calculate_bandwidth(
                         stats.filtered_min, N)
                  << " GB/s\n";

        std::cout << "Using mean time:    "
                  << calculate_bandwidth(
                         stats.filtered_mean, N)
                  << " GB/s\n";

        std::cout << "Using median time:  "
                  << calculate_bandwidth(
                         stats.filtered_median, N)
                  << " GB/s\n";
    }
}

#endif