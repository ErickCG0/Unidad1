#include <memory>
#include <string>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>
#include <chrono>

struct Fork {
    bool isTaken = false;
    std::string_view philosopher;
    std::mutex forkMutex;
    std::condition_variable taken;

    void takeFork(std::string_view philosopherName, std::string_view hand) {
        {
            std::unique_lock lock(forkMutex);
            taken.wait(lock, [&] {
                //std::println("{} is waiting for {} hand fork", philosopherName, hand);
                std::cout << philosopherName << " is waiting for " << hand << "fork" << std::endl;
                return !isTaken;
            });
            //std::println("{} is taking {} hand fork", philosopherName, hand);
            std::cout << philosopherName << " is taking " << hand << "fork" << std::endl;

            isTaken = true;
            philosopher = philosopherName;
        }
    }

    void releaseFork(std::string_view hand) {
        //std::println("{} is releasing {} hand fork", philosopher, hand);
        std::cout << philosopher << " is releasing " << hand << "fork" << std::endl;

        isTaken = false;
        this->philosopher = "";
        taken.notify_all();
    }


};

class Philosopher {
    std::string name;
    std::shared_ptr <Philosopher> rightPhilosopher = nullptr;
    std::shared_ptr <Fork> leftFork;
    std::shared_ptr <Fork> rightFork;

public:
    Philosopher(std::string name) : name(name) {
    }

    std::shared_ptr<Fork> RightFork() const {
        return rightFork;
    }
    std::shared_ptr<Fork> &RightFork() {
        return rightFork;
    }

    std::shared_ptr<Fork> LeftFork() const {
        return leftFork;
    }
    std::shared_ptr<Fork> &LeftFork() {
        return leftFork;
    }

    void eat() {
        while(true) {
            leftFork -> takeFork(name, "left");
            // std::printIN("{} took left fork", name);
            if (rightFork -> isTaken) {
                leftFork -> releaseFork("left");
                continue;
            }
            rightFork->takeFork(name, "right");
            //std::println("{} started eating", name);
            std::cout << name << " is eating" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            //std::println("{} done eating", name);
            std::cout << name << " done eating" << std::endl;
            rightFork->releaseFork("right");
            leftFork->releaseFork("left");
        }
    }
};

int main() {
    std::shared_ptr<Fork> fork1 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork2 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork3 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork4 = std::make_shared<Fork>();
    std::shared_ptr<Fork> fork5 = std::make_shared<Fork>();

    std::shared_ptr<Philosopher> socrates = std::make_shared<Philosopher>("Socrates");
    socrates -> LeftFork() = fork1;
    socrates -> RightFork() = fork2;

    std::shared_ptr<Philosopher> platon = std::make_shared<Philosopher>("Platon");
    platon -> LeftFork() = fork2;
    platon -> RightFork() = fork3;

    std::shared_ptr<Philosopher> aristotle = std::make_shared<Philosopher>("Aristotle");
    aristotle -> LeftFork() = fork3;
    aristotle -> RightFork() = fork4;

    std::shared_ptr<Philosopher> satyrus = std::make_shared<Philosopher>("Satyrus");
    satyrus -> LeftFork() = fork4;
    satyrus -> RightFork() = fork5;

    std::shared_ptr<Philosopher> thales = std::make_shared<Philosopher>("Thales");
    thales -> LeftFork() = fork5;
    thales -> RightFork() = fork1;

    std::thread threadSocrates([&]() { socrates -> eat();});
    std::thread threadPlaton([&]() { platon -> eat();});
    std::thread threadAristotle([&]() { aristotle -> eat();});
    std::thread threadSatyrus([&]() { satyrus -> eat();});
    std::thread threadThales([&]() { thales -> eat();});

    threadSocrates.join();
    threadPlaton.join();
    threadAristotle.join();
    threadSatyrus.join();
    threadThales.join();

    return 0;

}