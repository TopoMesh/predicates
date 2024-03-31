#include <Eigen/Dense>
#include <predicates/predicates.hpp>
#include <chrono>
#include <random>

using std::size_t;


template <size_t dimension, bool compute_pred>
auto benchmark(auto& rng, auto& engine, size_t num_trials) {
    const auto start_time = std::chrono::high_resolution_clock::now();
    for (size_t trial = 0; trial < num_trials; ++trial) {
        Eigen::Matrix<double, dimension + 1, dimension + 1> matrix;
        matrix.row(0).setConstant(1);
        for (size_t n = 0; n <= dimension; ++n)
            for (size_t m = 1; m <= dimension; ++m)
                matrix(m, n) = rng(engine);

        if constexpr(compute_pred)
            predicates::determinant(matrix);
    }
    const auto stop_time = std::chrono::high_resolution_clock::now();
    return stop_time - start_time;
}


int main() {
    std::random_device device;
    std::default_random_engine engine(device());
    const double mean = 0.0;
    const double stddev = 1.0;
    std::normal_distribution<double> rng(mean, stddev);

    {
        std::cout << "Dimension 2. Time for:\n";
        constexpr size_t dimension = 2;
        const size_t num_trials = 100000;
        const auto rng_duration = benchmark<dimension, false>(rng, engine, num_trials);
        const auto pred_duration = benchmark<dimension, true>(rng, engine, num_trials);
        using unit = std::chrono::nanoseconds;
        std::cout << "    generating random numbers:  "
                  << rng_duration.count() / (1e3 * num_trials) << " μs / iter\n";
        std::cout << "    RNG + computing predicates: "
                  << pred_duration.count() / (1e3 * num_trials) << " μs / iter\n";
    }

    {
        std::cout << "Dimension 3. Time for:\n";
        constexpr size_t dimension = 3;
        const size_t num_trials = 50000;
        const auto rng_duration = benchmark<dimension, false>(rng, engine, num_trials);
        const auto pred_duration = benchmark<dimension, true>(rng, engine, num_trials);
        using unit = std::chrono::nanoseconds;
        std::cout << "    generating random numbers:  "
                  << rng_duration.count() / (1e3 * num_trials) << " μs / iter\n";
        std::cout << "    RNG + computing predicates: "
                  << pred_duration.count() / (1e3 * num_trials) << " μs / iter\n";
    }

    {
        std::cout << "Dimension 4. Time for:\n";
        constexpr size_t dimension = 4;
        const size_t num_trials = 12500;
        const auto rng_duration = benchmark<dimension, false>(rng, engine, num_trials);
        const auto pred_duration = benchmark<dimension, true>(rng, engine, num_trials);
        using unit = std::chrono::nanoseconds;
        std::cout << "    generating random numbers:  "
                  << rng_duration.count() / (1e3 * num_trials) << " μs / iter\n";
        std::cout << "    RNG + computing predicates: "
                  << pred_duration.count() / (1e3 * num_trials) << " μs / iter\n";
    }


    return 0;
}
