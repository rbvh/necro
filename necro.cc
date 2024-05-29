#include <iostream>
#include <vector>
#include <string>
#include <memory> 
#include <fstream>
#include <sstream>
#include <random>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>

using namespace std;

bool CANTOR_IN_DECK = false;
bool GOOSE_IN_DECK = false;

struct BoardState;

struct Card {
    string color = "";
    string name = "";

    virtual vector<BoardState> act(const BoardState& state) {
        return {};
    }

    virtual unique_ptr<Card> clone() const = 0; 
    virtual ~Card() = default;
};

// Ensure the BoardState class has the appropriate operator== and sortHand method as before
struct BoardState {
    int black = 0;
    int redgreen = 0;
    int blue = 0;
    int landdrop = true;
    vector<unique_ptr<Card>> hand;
    bool bargain = false;
    bool win = false;

    BoardState(const BoardState& other): 
            black(other.black),
            redgreen(other.redgreen),
            blue(other.blue),
            landdrop(other.landdrop),
            bargain(other.bargain),
            win(other.win) {
        hand.reserve(other.hand.size());
        for (const auto& card : other.hand) {
            hand.push_back(card->clone());
        }
        sortHand();
    }

    BoardState& operator=(const BoardState& other) {
        if (this == &other) return *this;
        black = other.black;
        redgreen = other.redgreen;
        blue = other.blue;
        landdrop = other.landdrop;
        bargain = other.bargain;
        win = other.win;

        hand.clear();
        hand.reserve(other.hand.size());
        for (const auto& card : other.hand) {
            hand.push_back(card->clone());
        }
        sortHand();
        return *this;
    }

    BoardState() = default;

    void sortHand() {
        sort(hand.begin(), hand.end(), [](const unique_ptr<Card>& a, const unique_ptr<Card>& b) {
            return a->name < b->name;
        });
    }

    void print() const {
        cout << "BoardState(black=" << black
                << ", redgreen=" << redgreen
                << ", blue=" << blue
                << ", landdrop=" << landdrop
                << ", bargain=" << bargain
                << ", win=" << win
                << ", hand=[";
        for (const auto& card : hand) {
            cout << card->name << " ";
        }
        cout << "])" << endl;
    }

    bool operator==(const BoardState& other) const {
        if (black != other.black || redgreen != other.redgreen || blue != other.blue || landdrop != other.landdrop || bargain != other.bargain || win != other.win) {
            return false;
        }
        if (hand.size() != other.hand.size()) {
            return false;
        }
        for (size_t i = 0; i < hand.size(); ++i) {
            if (hand[i]->name != other.hand[i]->name) {
                return false;
            }
        }
        return true;
    }
};

struct BoardStateHash {
    size_t operator()(const BoardState& state) const {
        size_t res = 17;
        res = res * 31 + hash<int>()(state.black);
        res = res * 31 + hash<int>()(state.redgreen);
        res = res * 31 + hash<int>()(state.blue);
        res = res * 31 + hash<bool>()(state.landdrop);
        res = res * 31 + hash<bool>()(state.bargain);
        res = res * 31 + hash<bool>()(state.win);
        for (const auto& card : state.hand) {
            res = res * 31 + hash<string>()(card->name);
        }
        return res;
    }
};

// -------------------------------------------------------------------
// -------------------------------------------------------------------
// -------------------------------------------------------------------

struct DarkRitual : Card {
    DarkRitual() { name = "DarkRitual"; color = "black"; }
    
    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.black >= 1) {
            BoardState new_state = state;
            new_state.black += 2;
            result.push_back(new_state);
        }
        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<DarkRitual>(*this);
    }
};

struct CabalRitual : Card { 
    CabalRitual() { name = "CabalRitual"; color = "black"; }
    
    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.black >= 2) {
            BoardState new_state = state;
            new_state.black += 1;
            result.push_back(new_state);
        }

        if (state.black >= 1 && state.redgreen >= 1) {
            BoardState new_state = state;
            new_state.black += 2;
            new_state.redgreen -= 1;
            result.push_back(new_state);
        }

        if (state.black >= 1 && state.blue >= 1) {
            BoardState new_state = state;
            new_state.black += 2;
            new_state.blue -= 1;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<CabalRitual>(*this);
    }
};

struct SpiritGuide : Card {
    SpiritGuide() { name = "SpiritGuide"; color = "redgreen"; }
    
    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (true) {
            BoardState new_state = state;
            new_state.redgreen += 1;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<SpiritGuide>(*this);
    }
};

struct VaultOfWhispers : Card {
    VaultOfWhispers() { name = "VaultOfWhispers"; color = ""; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.landdrop) {
            BoardState new_state = state;
            new_state.landdrop = false;
            new_state.black += 1;
            new_state.bargain = true;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<VaultOfWhispers>(*this);
    }
};

struct TreeOfTales : Card {
    TreeOfTales() { name = "TreeOfTales"; color = ""; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.landdrop) {
            BoardState new_state = state;
            new_state.landdrop = false;
            new_state.redgreen += 1;
            new_state.bargain = true;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<TreeOfTales>(*this);
    }
};

struct GemstoneMine : Card {
    GemstoneMine() { name = "GemstoneMine"; color = ""; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.landdrop) {
            BoardState new_state = state;
            new_state.landdrop = false;
            new_state.black += 1;
            result.push_back(new_state);
        }

        if (state.landdrop) {
            BoardState new_state = state;
            new_state.landdrop = false;
            new_state.redgreen += 1;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<GemstoneMine>(*this);
    }
};

struct Boseiju : Card {
    Boseiju() { name = "Boseiju"; color = ""; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.landdrop) {
            BoardState new_state = state;
            new_state.landdrop = false;
            new_state.redgreen += 1;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Boseiju>(*this);
    }
};

struct Valakut : Card {
    Valakut() { name = "Valakut"; color = "redgreen"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Valakut>(*this);
    }
};

struct WildCantor : Card {
    WildCantor() { name = "WildCantor"; color = "redgreen"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.redgreen >= 1) {
            BoardState new_state = state;
            new_state.redgreen -= 1;
            new_state.black += 1;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<WildCantor>(*this);
    }
};

struct Goose : Card {
    Goose() { name = "Goose"; color = "redgreen"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.redgreen >= 1) {
            BoardState new_state = state;
            new_state.redgreen -= 1;
            new_state.bargain = true;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Goose>(*this);
    }
};

struct LotusPetal : Card {
    LotusPetal() { name = "LotusPetal"; color = ""; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (true) {
            BoardState new_state = state;
            new_state.black += 1;
            result.push_back(new_state);
        }
        if (true) {
            BoardState new_state = state;
            new_state.redgreen += 1;
            result.push_back(new_state);
        }
        if (true) {
            BoardState new_state = state;
            new_state.bargain = true;
            result.push_back(new_state);
        }
        
        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<LotusPetal>(*this);
    }
};

struct ChromeMox : Card {
    ChromeMox() { name = "ChromeMox"; color = ""; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        for (int i = 0; i < state.hand.size(); ++i) {
            if (!state.hand[i]->color.empty()) {
                BoardState new_state = state;
                new_state.hand.clear();
                for (int j = 0; j < state.hand.size(); ++j) {
                    if (j != i) {
                        new_state.hand.push_back(state.hand[j]->clone());
                    }
                }

                new_state.bargain = true;
                if (state.hand[i]->color == "black") {
                    new_state.black += 1;
                } else if (state.hand[i]->color == "redgreen") {
                    new_state.redgreen += 1;
                } else if (state.hand[i]->color == "blue") {
                    new_state.blue += 1;
                }
                result.push_back(new_state);
            }
        }

        BoardState new_state = state;
        new_state.bargain += 1;
        result.push_back(new_state);

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<ChromeMox>(*this);
    }
};

struct SummonersPact : Card {
    SummonersPact() { name = "SummonersPact"; color = "redgreen"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (true) {
            BoardState new_state = state;
            new_state.redgreen += 1;
            result.push_back(new_state);
        }

        if (state.redgreen >= 1 && CANTOR_IN_DECK) {
            BoardState new_state = state;
            new_state.redgreen -= 1;
            new_state.black += 1;
            result.push_back(new_state);
        }

        if (state.redgreen >= 1 && GOOSE_IN_DECK) {
            BoardState new_state = state;
            new_state.redgreen -= 1;
            new_state.bargain = true;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<SummonersPact>(*this);
    }
};

struct Necro : Card {
    Necro() { name = "Necro"; color = "black"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.black >= 3) {
            BoardState new_state = state;
            new_state.black -= 3;
            new_state.win = true;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Necro>(*this);
    }
};

struct Beseech : Card {
    Beseech() { name = "Beseech"; color = "black"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        // Bargain lines
        if (state.bargain) {
            if (state.black >= 4) {
                BoardState new_state = state;
                new_state.black -= 4;
                new_state.win = true;
                result.push_back(new_state);
            }
            if (state.black >= 3 && state.redgreen >= 1) {
                BoardState new_state = state;
                new_state.black -= 3;
                new_state.redgreen -= 1;
                new_state.win = true;
                result.push_back(new_state);
            }
            if (state.black >= 3 && state.blue >= 1) {
                BoardState new_state = state;
                new_state.black -= 3;
                new_state.blue -= 1;
                new_state.win = true;
                result.push_back(new_state);
            }
        }

        // No-bargain lines
        if (state.black >= 7) {
            BoardState new_state = state;
            new_state.black -= 7;
            new_state.win = true;
            result.push_back(new_state);
        }
        if (state.black >= 6 && state.redgreen >= 1) {
            BoardState new_state = state;
            new_state.black -= 6;
            new_state.redgreen -= 1;
            new_state.win = true;
            result.push_back(new_state);
        }
        if (state.black >= 6 && state.blue >= 1) {
            BoardState new_state = state;
            new_state.black -= 6;
            new_state.blue -= 1;
            new_state.win = true;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Beseech>(*this);
    }
};

struct Necrologia : Card {
    Necrologia() { name = "Necrologia"; color = "black"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.black >= 5) {
            BoardState new_state = state;
            new_state.black -= 5;
            new_state.win = true;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Necrologia>(*this);
    }
};

struct Manamorphose : Card {
    Manamorphose() { name = "Manamorphose"; color = "redgreen"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        if (state.redgreen >= 2) {
            BoardState new_state = state;
            new_state.redgreen -= 2;
            new_state.black += 2;
            result.push_back(new_state);
        }

        if (state.redgreen >= 1 && state.black >= 1) {
            BoardState new_state = state;
            new_state.redgreen -= 1;
            new_state.black += 1;
            result.push_back(new_state);
        }

        if (state.redgreen >= 1 && state.blue >= 1) {
            BoardState new_state = state;
            new_state.redgreen -= 1;
            new_state.blue -= 1;
            new_state.black += 2;
            result.push_back(new_state);
        }

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Manamorphose>(*this);
    }
};

struct Borne : Card {
    Borne() { name = "Borne"; color = "blue"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Borne>(*this);
    }
};

struct PactOfNegation : Card {
    PactOfNegation() { name = "PactOfNegation"; color = "blue"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<PactOfNegation>(*this);
    }
};

struct Tendrils : Card {
    Tendrils() { name = "Tendrils"; }

    vector<BoardState> act(const BoardState& state) override {
        vector<BoardState> result;

        return result;
    }

    unique_ptr<Card> clone() const override {
        return make_unique<Tendrils>(*this);
    }
};

void recurse(const BoardState& state, vector<BoardState> &winning_states) {
    if (state.win) {
        winning_states.push_back(state);
        return;
    }

    for (int i = 0; i < state.hand.size(); i++) {
        BoardState new_state = state;
        new_state.hand.clear();  

        // Repopulate the hand vector with clones of the cards
        for (int j = 0; j < state.hand.size(); j++) {
            if (j != i) {
                new_state.hand.push_back(state.hand[j]->clone());
            }
        }

        auto new_states = state.hand[i]->act(new_state);

        for (const auto& new_state : new_states) {
            recurse(new_state, winning_states);
        }
    }
}

unique_ptr<Card> createCard(const string& name) {
    if (name == "DarkRitual") return make_unique<DarkRitual>();
    if (name == "CabalRitual") return make_unique<CabalRitual>();
    if (name == "SpiritGuide") return make_unique<SpiritGuide>();
    if (name == "VaultOfWhispers") return make_unique<VaultOfWhispers>();
    if (name == "TreeOfTales") return make_unique<TreeOfTales>();
    if (name == "GemstoneMine") return make_unique<GemstoneMine>();
    if (name == "Boseiju") return make_unique<Boseiju>();
    if (name == "Valakut") return make_unique<Valakut>();
    if (name == "LotusPetal") return make_unique<LotusPetal>();
    if (name == "ChromeMox") return make_unique<ChromeMox>();
    if (name == "SummonersPact") return make_unique<SummonersPact>();
    if (name == "Necro") return make_unique<Necro>();
    if (name == "Beseech") return make_unique<Beseech>();
    if (name == "Necrologia") return make_unique<Necrologia>();
    if (name == "Manamorphose") return make_unique<Manamorphose>();
    if (name == "Borne") return make_unique<Borne>();
    if (name == "PactOfNegation") return make_unique<PactOfNegation>();
    if (name == "Tendrils") return make_unique<Tendrils>();

    if (name == "WildCantor") {
        CANTOR_IN_DECK = true;
        return make_unique<WildCantor>();
    }
    if (name == "Goose") {
        GOOSE_IN_DECK = true;
        return make_unique<Goose>();
    }
    
    return nullptr;
}

vector<unique_ptr<Card>> readDeckFromFile(const string& filename) {
    vector<unique_ptr<Card>> deck;
    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        int count;
        string name;
        if (!(iss >> count >> name)) { break; }

        for (int i = 0; i < count; ++i) {
            auto card = createCard(name);
            if (card) {
                deck.push_back(std::move(card));
            }
        }
    }

    return deck;
}

void bfs(const BoardState& initial_state, vector<BoardState> &winning_states) {
    queue<BoardState> q;
    unordered_set<BoardState, BoardStateHash> visited;

    q.push(initial_state);
    visited.insert(initial_state);

    while (!q.empty()) {
        BoardState state = q.front();
        q.pop();

        if (state.win) {
            winning_states.push_back(state);
            continue;
        }

        for (int i = 0; i < state.hand.size(); i++) {
            BoardState new_state = state;
            new_state.hand.clear();  

            for (int j = 0; j < state.hand.size(); j++) {
                if (j != i) {
                    new_state.hand.push_back(state.hand[j]->clone());
                }
            }

            auto new_states = state.hand[i]->act(new_state);

            for (auto& next_state : new_states) {
                next_state.sortHand(); // Ensure hand is sorted for each new state
                if (visited.find(next_state) == visited.end()) {
                    visited.insert(next_state);
                    q.push(next_state);
                }
            }
        }
    }
}

void processChunk(int n, const vector<unique_ptr<Card>>& deck, vector<long>& win_counts, mutex& mtx) {
    random_device rd;
    mt19937 generator(rd());
    vector<int> indices(deck.size());
    iota(indices.begin(), indices.end(), 0);

    vector<long> local_win_counts(7, 0.0);

    for (int i = 0; i < n; i++) {
        shuffle(indices.begin(), indices.end(), generator);

        vector<unique_ptr<Card>> hand;
        for (int j = 0; j < 7; j++) {
            hand.push_back(deck[indices[j]]->clone());
        }

        BoardState state;
        state.hand = std::move(hand);
        state.sortHand();

        vector<BoardState> winning_states;

        bfs(state, winning_states);

        if (winning_states.empty()) {
            continue;
        }

        // Count number of cards left
        unsigned long cards_left = 0;
        for (const auto& state : winning_states) {
            cards_left = max(state.hand.size(), cards_left);
        }

        int cards_used = 6 - cards_left;
        for (int j = cards_used; j < 7; j++) {
            local_win_counts[j] += 1;
        }
    }

    // Safely update the shared win_rates vector
    lock_guard<mutex> lock(mtx);
    for (int j = 0; j < 7; j++) {
        win_counts[j] += local_win_counts[j];
    }
}
int main() {
    vector<unique_ptr<Card>> deck = readDeckFromFile("decklists/test.txt");

    cout << deck.size() << endl;

    int n = 100000;
    int num_threads = thread::hardware_concurrency(); // Get the number of supported threads

    while(n % num_threads != 0) {
        n++;
    }
    int chunk_size = n / num_threads;

    vector<long> win_counts(7, 0);
    mutex mtx;
    vector<thread> threads;

    // Start threads to process chunks
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(processChunk, chunk_size, ref(deck), ref(win_counts), ref(mtx));
    }

    // Join threads
    for (auto& t : threads) {
        t.join();
    }

    // Process any remaining iterations
    int remaining_iterations = n % num_threads;
    if (remaining_iterations > 0) {
        processChunk(remaining_iterations, deck, win_counts, mtx);
    }

    vector<double> win_rates(7, 0.0);
    vector<double> win_rate_errors(7, 0.0);
    for (int i = 0; i < 7; i++) {
        win_rates[i] = win_counts[i] / (double)n;
        win_rate_errors[i] = sqrt(win_rates[i] * (1 - win_rates[i]) / n);
    }

    vector<double> win_rates_cumulative(7, 0.0);
    win_rates_cumulative[0] = win_rates[0];
    for (int i=1; i<7; i++) {
        win_rates_cumulative[i] = win_rates[i] + (1 - win_rates[i]) * win_rates_cumulative[i-1];
    }

    vector<double> win_rate_partials(7, 0.0);
    win_rate_partials[6] = 1 - win_rates_cumulative[5];
    for (int i=5; i>=1; i--) {
        win_rate_partials[i] = (1 - win_rates_cumulative[i-1]);
        for (int j=i; j<6; j++) {
            win_rate_partials[i] = win_rates[j] + (1 - win_rates[j]) * win_rate_partials[i];
        }
    }

    double win_rate_total = win_rates_cumulative[6];
    double win_rate_total_error = 0;
    for (int i=0; i<7; i++) {
        win_rate_total_error += pow(win_rate_errors[i] * win_rate_partials[i], 2);
    }
    win_rate_total_error = sqrt(win_rate_total_error);

    cout << "win_rates: " << endl;
    cout << "1 card: " << 100*win_rates[0] << "% \u00B1 " << 100*win_rate_errors[0] << "%" << endl;
    cout << "2 cards: " << 100*win_rates[1] << "% \u00B1 " << 100*win_rate_errors[1] << "%" << endl;
    cout << "3 cards: " << 100*win_rates[2] << "% \u00B1 " << 100*win_rate_errors[2] << "%" << endl;
    cout << "4 cards: " << 100*win_rates[3] << "% \u00B1 " << 100*win_rate_errors[3] << "%" << endl;
    cout << "5 cards: " << 100*win_rates[4] << "% \u00B1 " << 100*win_rate_errors[4] << "%" << endl;
    cout << "6 cards: " << 100*win_rates[5] << "% \u00B1 " << 100*win_rate_errors[5] << "%" << endl;
    cout << "7 cards: " << 100*win_rates[6] << "% \u00B1 " << 100*win_rate_errors[6] << "%" << endl;
    cout << "Total winrate: " << 100*win_rate_total << "% \u00B1 " << 100*win_rate_total_error << "%" << endl;

    return 0;
}
