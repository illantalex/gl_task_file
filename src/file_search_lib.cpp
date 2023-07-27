#include "file_search_lib.h"
#include <iostream>
#include <thread>
#include <filesystem>
#include <mutex>
#include <unordered_set>

namespace fs = std::filesystem;

namespace FileSearch {
    static bool found = false;
    static std::mutex foundMutex;

    static const std::unordered_set<std::string> excludedDirs = {
            "/proc",
            "/sys",
            "/run"
            // Add other directories to exclude here if needed
    };

    bool shouldExclude(const fs::path& path) {
        return excludedDirs.find(path.string()) != excludedDirs.end();
    }

    void searchFileRecursive(const fs::path& directory, const std::string& fileName, int maxThreads) {
        if (found) return;

        if (shouldExclude(directory)) return;

        if (fs::is_symlink(directory)) return; // Skip symbolic links

        std::vector<std::thread> threads;
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (fs::is_directory(entry)) {
                if (threads.size() < maxThreads) {
                    threads.emplace_back(searchFileRecursive, entry, fileName, maxThreads);
                } else {
                    searchFileRecursive(entry, fileName, maxThreads);
                }
            } else if (entry.path().filename() == fileName) {
                std::unique_lock<std::mutex> lock(foundMutex);
                if (!found) {
                    std::cout << "Found at: " << entry.path().string() << std::endl;
                    found = true;
                }
            }
        }

        for (auto& thread : threads) {
            thread.join();
        }
    }

    void searchFile(const std::string& rootDir, const std::string& fileName) {
        found = false;
        fs::path rootPath(rootDir);
        if (fs::exists(rootPath) && fs::is_directory(rootPath)) {
            int maxThreads = 8;
            searchFileRecursive(rootPath, fileName, maxThreads);
        } else {
            std::cerr << "Error: Invalid root directory." << std::endl;
        }
    }
}
