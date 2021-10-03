#include <algorithm>
#include <set>

template<typename T>
class DistinctObjectPairTracker
{
public:
    void Add(T& objA, T& objB)
    {
        _objects.emplace(
            std::min(reinterpret_cast<uintptr_t>(&objA), reinterpret_cast<uintptr_t>(&objB)),
            std::max(reinterpret_cast<uintptr_t>(&objA), reinterpret_cast<uintptr_t>(&objB))
        );
    }

    bool Contains(T& objA, T& objB)
    {
        return _objects.contains(
            std::tuple<uintptr_t, uintptr_t>(
                std::min(reinterpret_cast<uintptr_t>(&objA), reinterpret_cast<uintptr_t>(&objB)),
                std::max(reinterpret_cast<uintptr_t>(&objA), reinterpret_cast<uintptr_t>(&objB))
            )
        );
    }

    size_t Size()
    {
        return _objects.size();
    }

private:
    std::set<std::tuple<uintptr_t, uintptr_t>> _objects;
};
