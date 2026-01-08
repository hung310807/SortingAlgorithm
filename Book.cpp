#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <optional>

using namespace std;

enum class Role { ROOT, CHAPTER, SECTION, SUBSECTION, SUBSUBSECTION };

static const char* roleName(Role r) {
    switch (r) {
        case Role::ROOT: return "ROOT";
        case Role::CHAPTER: return "CHAPTER";
        case Role::SECTION: return "SECTION";
        case Role::SUBSECTION: return "SUBSECTION";
        case Role::SUBSUBSECTION: return "SUBSUBSECTION";
    }
    return "UNKNOWN";
}

struct Node {
    Role role{Role::ROOT};
    string id;      
    string title;
    int startPage{0};
    int endPage{0};

    Node* parent{nullptr};                       
    vector<unique_ptr<Node>> children;           

    Node(Role role, string id, string title, int startPage, int endPage, Node* parent=nullptr)
        : role(role), id(std::move(id)), title(std::move(title)),
          startPage(startPage), endPage(endPage), parent(parent) {}

    int pageCount() const {
        if (startPage <= 0 || endPage < startPage) return 0;
        return endPage - startPage + 1;
    }

    Node* addChild(unique_ptr<Node> child) {
        child->parent = this;
        children.push_back(std::move(child));
        return children.back().get();
    }
};

struct DeleteInfo {
    int delStart{0};
    int delEnd{0};
    int removedPages{0};
};


static Node* findById(Node* root, const string& id) {
    if (!root) return nullptr;
    if (root->id == id) return root;
    for (auto& ch : root->children) {
        if (Node* got = findById(ch.get(), id)) return got;
    }
    return nullptr;
}

static optional<pair<Node*, size_t>> findParentAndIndex(Node* root, const string& targetId) {
    if (!root) return nullopt;
    for (size_t i = 0; i < root->children.size(); ++i) {
        if (root->children[i] && root->children[i]->id == targetId) {
            return make_pair(root, i);
        }
        if (auto got = findParentAndIndex(root->children[i].get(), targetId)) return got;
    }
    return nullopt;
}

static void updatePagesAfterDeletion(Node* root, const DeleteInfo& d) {
    if (!root) return;

    if (root->startPage > d.delEnd) {
        root->startPage -= d.removedPages;
        root->endPage   -= d.removedPages;
    }
    else if (root->startPage <= d.delStart && root->endPage >= d.delEnd) {
        root->endPage -= d.removedPages;
    }

    for (auto& ch : root->children) updatePagesAfterDeletion(ch.get(), d);
}

static int countChapters(const Node* root) {
    if (!root) return 0;
    int cnt = 0;
    for (const auto& ch : root->children) {
        if (ch && ch->role == Role::CHAPTER) cnt++;
    }
    return cnt;
}

static const Node* longestChapter(const Node* root) {
    if (!root) return nullptr;
    const Node* best = nullptr;
    int bestLen = -1;

    for (const auto& ch : root->children) {
        if (!ch || ch->role != Role::CHAPTER) continue;
        int len = ch->pageCount();
        if (len > bestLen) {
            bestLen = len;
            best = ch.get();
        }
    }
    return best;
}


static vector<int> computeNumberPath(const Node* node) {
    vector<int> path;
    const Node* cur = node;
    while (cur && cur->parent) {
        const Node* p = cur->parent;
        if (p->role == Role::ROOT) {
            int idx = 0;
            for (const auto& s : p->children) {
                if (!s) continue;
                if (s->role == cur->role) idx++;
                if (s.get() == cur) { path.push_back(idx); break; }
            }
        } else {
            int idx = 0;
            for (const auto& s : p->children) {
                if (!s) continue;
                if (s->role == cur->role) idx++;
                if (s.get() == cur) { path.push_back(idx); break; }
            }
        }
        cur = cur->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

static string numberPathToString(const vector<int>& path) {
    if (path.empty()) return "";
    string s;
    for (size_t i = 0; i < path.size(); ++i) {
        s += to_string(path[i]);
        if (i + 1 < path.size()) s += '.';
    }
    return s;
}

static void printSubtreeHeadings(const Node* node, int indentSpaces) {
    if (!node) return;
    vector<int> path = computeNumberPath(node);
    string num = numberPathToString(path);

    for (int i = 0; i < indentSpaces; ++i) cout << ' ';
    if (!num.empty()) cout << num << " ";
    cout << node->title << " (p." << node->startPage << "-" << node->endPage << ", " << roleName(node->role) << ")\n";

    for (const auto& ch : node->children) {
        printSubtreeHeadings(ch.get(), indentSpaces + 2);
    }
}

static void printChapterHeadings(Node* root, const string& chapterId) {
    Node* ch = findById(root, chapterId);
    if (!ch || ch->role != Role::CHAPTER) {
        cout << "Chapter not found or id is not a CHAPTER.\n";
        return;
    }
    printSubtreeHeadings(ch, 0);
}

static bool deleteNodeById(Node* root, const string& targetId) {
    if (!root) return false;

    if (root->id == targetId) return false;

    auto loc = findParentAndIndex(root, targetId);
    if (!loc) return false;

    Node* parent = loc->first;
    size_t idx = loc->second;

    Node* victim = parent->children[idx].get();
    if (!victim) return false;

    DeleteInfo d;
    d.delStart = victim->startPage;
    d.delEnd = victim->endPage;
    d.removedPages = victim->pageCount();

    parent->children.erase(parent->children.begin() + static_cast<long>(idx));

    if (d.removedPages > 0) updatePagesAfterDeletion(root, d);
    return true;
}

static unique_ptr<Node> buildExampleBook() {
    auto root = make_unique<Node>(Role::ROOT, "root", "BOOK", 1, 120, nullptr);

    // Chapter 1: pages 1-40
    Node* ch1 = root->addChild(make_unique<Node>(Role::CHAPTER, "ch1", "Chapter 1: Intro", 1, 40));
    Node* ch1_s1 = ch1->addChild(make_unique<Node>(Role::SECTION, "ch1_s1", "1.1 Basics", 1, 12));
    ch1_s1->addChild(make_unique<Node>(Role::SUBSECTION, "ch1_s1_ss1", "1.1.1 Definitions", 1, 6));
    ch1_s1->addChild(make_unique<Node>(Role::SUBSECTION, "ch1_s1_ss2", "1.1.2 Examples", 7, 12));
    Node* ch1_s2 = ch1->addChild(make_unique<Node>(Role::SECTION, "ch1_s2", "1.2 Tools", 13, 40));
    ch1_s2->addChild(make_unique<Node>(Role::SUBSECTION, "ch1_s2_ss1", "1.2.1 Setup", 13, 25));
    ch1_s2->addChild(make_unique<Node>(Role::SUBSECTION, "ch1_s2_ss2", "1.2.2 Usage", 26, 40));

    // Chapter 2: pages 41-120
    Node* ch2 = root->addChild(make_unique<Node>(Role::CHAPTER, "ch2", "Chapter 2: Main", 41, 120));
    Node* ch2_s1 = ch2->addChild(make_unique<Node>(Role::SECTION, "ch2_s1", "2.1 Theory", 41, 80));
    ch2_s1->addChild(make_unique<Node>(Role::SUBSECTION, "ch2_s1_ss1", "2.1.1 Core", 41, 60));
    ch2_s1->addChild(make_unique<Node>(Role::SUBSECTION, "ch2_s1_ss2", "2.1.2 Extensions", 61, 80));
    Node* ch2_s2 = ch2->addChild(make_unique<Node>(Role::SECTION, "ch2_s2", "2.2 Practice", 81, 120));
    ch2_s2->addChild(make_unique<Node>(Role::SUBSECTION, "ch2_s2_ss1", "2.2.1 Exercises", 81, 100));
    ch2_s2->addChild(make_unique<Node>(Role::SUBSECTION, "ch2_s2_ss2", "2.2.2 Projects", 101, 120));

    return root;
}

int main() {
    auto book = buildExampleBook();
    cout << "Number of chapters: " << countChapters(book.get()) << "\n\n";
    if (const Node* best = longestChapter(book.get())) {
        cout << "Longest chapter: " << best->title
             << " (pages " << best->startPage << "-" << best->endPage
             << ", len=" << best->pageCount() << ")\n\n";
    }

    cout << "Headings of ch2:\n";
    printChapterHeadings(book.get(), "ch2");
    cout << "\n";

    cout << "Deleting node ch1_s2...\n";
    bool ok = deleteNodeById(book.get(), "ch1_s2");
    cout << (ok ? "Delete OK\n\n" : "Delete FAILED\n\n");

    cout << "After deletion, headings of ch2:\n";
    printChapterHeadings(book.get(), "ch2");
    cout << "\n";

    cout << "Number of chapters: " << countChapters(book.get()) << "\n";
    return 0;
}
