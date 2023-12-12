#include <QApplication>
#include "MainWindow.h"

#include "thread_pool.h"
#include <future>
#include <iterator>

#include <iostream>


void task()
{
    std::cout << "async task 21\n";
}

int task1()
{
    std::cout <<"async task with return\n";

    return 66798;
}

std::string complexTask(int value, const std::string& context)
{
    return "complex task returned: " + context + ' ' + std::to_string(value);
}


template<typename T>
auto wait_for(std::vector<std::future<T>>& futures) -> std::vector<T>
{
    size_t count = futures.size();

    std::vector<T> results;
    results.reserve(futures.size());

    while (count > 0)
    {
        for (auto& f : futures)
        {
            if (!f.valid())
                continue;

            if (f.wait_for(std::chrono::seconds::zero()) == std::future_status::ready)
            {
                --count;

                results.push_back(f.get()); // get() will invalidate future and the same one won't decrement counter multiple times
            }
        }
    }

    return results;
}


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

	MainWindow w;
	w.show();

    return app.exec();

    // TPool::async(task1);

    // auto asyncR = TPool::async(complexTask, 5, "main thread");

    // std::vector<std::future<std::string>> futures;
    // for (int i = 0; i < 10; ++i)
    // {
    //     auto r = TPool::async([i]() -> std::string { return "value from promise " + std::to_string(i*i); });

    //     futures.emplace_back(std::move(r));
    // }

    // std::cout << "Result from task with return value: " << asyncR.get() << '\n';
    
    // auto results = wait_for(futures);

    // for (auto r : results)
    // {
    //     std::cout << "result: " << r << '\n';
    // }


}
