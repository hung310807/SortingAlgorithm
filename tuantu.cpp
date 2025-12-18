#include <vector>
#include <functional>
#include <stdexcept>

template <typename T>
struct ArrayTree {
    using Index = std::size_t;
    static constexpr Index NPOS = static_cast<Index>(-1);
    std::vector<T>    data;
    std::vector<bool> used;
    Index root;
    void Init(std::size_t cap = 1) {
        data.assign(cap, T{});
        used.assign(cap, false);
        root = NPOS;
    }
    bool IsEmpty() const {
        return root == NPOS || !IsUsed(root);
    }
    Index SetRoot(const T& v, Index idx = 0) {
        Ensure(idx);
        root = idx;
        used[idx] = true;
        data[idx] = v;
        return idx;
    }
    Index AddLeft(Index parent, const T& v) {
        Require(parent);
        Index child = 2 * parent + 1;
        Ensure(child);
        used[child] = true;
        data[child] = v;
        return child;
    }
    Index AddRight(Index parent, const T& v) {
        Require(parent);
        Index child = 2 * parent + 2;
        Ensure(child);
        used[child] = true;
        data[child] = v;
        return child;
    }
    void PreOrder (const std::function<void(const T&)>& Visit) const { Pre (root, Visit); }
    void InOrder  (const std::function<void(const T&)>& Visit) const { In  (root, Visit); }
    void PostOrder(const std::function<void(const T&)>& Visit) const { Post(root, Visit); }
    bool IsUsed(Index i) const {
        return i < used.size() && used[i];
    }
    void Require(Index i) const {
        if (!IsUsed(i)) throw std::runtime_error("Invalid parent");
    }
    void Ensure(Index i) {
        if (i < used.size()) return;
        std::size_t cap = used.empty() ? 1 : used.size();
        while (cap <= i) cap <<= 1;
        used.resize(cap, false);
        data.resize(cap, T{});
    }
    void Pre(Index i, const std::function<void(const T&)>& Visit) const {
        if (!IsUsed(i)) return;
        Visit(data[i]);
        Pre(2 * i + 1, Visit);
        Pre(2 * i + 2, Visit);
    }
    void In(Index i, const std::function<void(const T&)>& Visit) const {
        if (!IsUsed(i)) return;
        In(2 * i + 1, Visit);
        Visit(data[i]);
        In(2 * i + 2, Visit);
    }
    void Post(Index i, const std::function<void(const T&)>& Visit) const {
        if (!IsUsed(i)) return;
        Post(2 * i + 1, Visit);
        Post(2 * i + 2, Visit);
        Visit(data[i]);
    }
};

int main() {
    ArrayTree<int> T;
    T.Init();
    auto R  = T.SetRoot(1);
    auto L  = T.AddLeft(R, 2);
    auto RR = T.AddRight(R, 3);
    T.AddLeft(L, 4);
    T.AddRight(L, 5);
    std::cout << "PreOrder : ";
    T.PreOrder([](const int& x){ std::cout << x << ' '; });
    std::cout << "\nInOrder  : ";
    T.InOrder([](const int& x){ std::cout << x << ' '; });
    std::cout << "\nPostOrder: ";
    T.PostOrder([](const int& x){ std::cout << x << ' '; });
    std::cout << "\n";
    return 0;
}
