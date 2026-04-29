#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <unordered_map>

/**
 * Problem 1: Dynamic Leaderboard
 */
struct Player {
    std::string name;
    int score;

    bool operator<(const Player& other) const {
        if (score != other.score) return score > other.score;
        return name < other.name;
    }
};

class Leaderboard {
private:
    std::unordered_map<std::string, int> player_scores;
    std::set<Player> ranking;

public:
    void add(const std::string& name, int score) {
        player_scores[name] = score;
        ranking.insert({name, score});
    }

    void update(const std::string& name, int delta) {
        if (player_scores.find(name) == player_scores.end()) return;

        ranking.erase({name, player_scores[name]});
        player_scores[name] += delta;
        ranking.insert({name, player_scores[name]});
    }

    void remove(const std::string& name) {
        if (player_scores.find(name) == player_scores.end()) return;

        ranking.erase({name, player_scores[name]});
        player_scores.erase(name);
    }

    void top(int k) {
        int count = 0;
        for (auto it = ranking.begin(); it != ranking.end() && count < k; ++it, ++count) {
            std::cout << it->name << " " << it->score << std::endl;
        }
        std::cout << std::endl;
    }
};

int main() {
    Leaderboard lb;
    lb.add("Alice", 120);
    lb.add("Bob", 90);
    lb.add("Carol", 150);
    lb.update("Bob", 50);
    lb.top(2);
    lb.remove("Carol");
    lb.top(2);
    return 0;
}
