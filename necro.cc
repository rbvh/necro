#include <iostream>
#include <vector>
#include <string>
#include <memory> 
#include <fstream>
#include <sstream>
#include <random>
#include <unordered_map>
#include <algorithm>
#include <thread>
#include <mutex>
#include <atomic>

using namespace std;

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
        return *this;
    }

    BoardState() = default;

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

        if (state.redgreen >= 1) {
            BoardState new_state = state;
            new_state.redgreen -= 1;
            new_state.black += 1;
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
    cout << name << endl;
    if (name == "DarkRitual") return make_unique<DarkRitual>();
    if (name == "CabalRitual") return make_unique<CabalRitual>();
    if (name == "SpiritGuide") return make_unique<SpiritGuide>();
    if (name == "VaultOfWhispers") return make_unique<VaultOfWhispers>();
    if (name == "GemstoneMine") return make_unique<GemstoneMine>();
    if (name == "Boseiju") return make_unique<Boseiju>();
    if (name == "Valakut") return make_unique<Valakut>();
    if (name == "WildCantor") return make_unique<WildCantor>();
    if (name == "LotusPetal") return make_unique<LotusPetal>();
    if (name == "ChromeMox") return make_unique<ChromeMox>();
    if (name == "SummonersPact") return make_unique<SummonersPact>();
    if (name == "Necro") return make_unique<Necro>();
    if (name == "Beseech") return make_unique<Beseech>();
    if (name == "Manamorphose") return make_unique<Manamorphose>();
    if (name == "Borne") return make_unique<Borne>();
    if (name == "PactOfNegation") return make_unique<PactOfNegation>();
    if (name == "Tendrils") return make_unique<Tendrils>();

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
        state.hand = move(hand);

        vector<BoardState> winning_states;

        recurse(state, winning_states);

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

    double win_rate_total = win_rates[6] 
                  + (1 - win_rates[6]) * win_rates[5] 
                  + (1 - win_rates[6]) * (1 - win_rates[5]) * win_rates[4] 
                  + (1 - win_rates[6]) * (1 - win_rates[5]) * (1 - win_rates[4]) * win_rates[3] 
                  + (1 - win_rates[6]) * (1 - win_rates[5]) * (1 - win_rates[4]) * (1 - win_rates[3]) * win_rates[2] 
                  + (1 - win_rates[6]) * (1 - win_rates[5]) * (1 - win_rates[4]) * (1 - win_rates[3]) * (1 - win_rates[2]) * win_rates[1] 
                  + (1 - win_rates[6]) * (1 - win_rates[5]) * (1 - win_rates[4]) * (1 - win_rates[3]) * (1 - win_rates[2]) * (1 - win_rates[1]) * win_rates[0];

    cout << "win_rates: " << endl;
    cout << "1 card: " << win_rates[0] << " \pm " << win_rate_errors[0] << endl;
    cout << "2 cards: " << win_rates[1] << " \pm " << win_rate_errors[1] << endl;
    cout << "3 cards: " << win_rates[2] << " \pm " << win_rate_errors[2] << endl;
    cout << "4 cards: " << win_rates[3] << " \pm " << win_rate_errors[3] << endl;
    cout << "5 cards: " << win_rates[4] << " \pm " << win_rate_errors[4] << endl;
    cout << "6 cards: " << win_rates[5] << " \pm " << win_rate_errors[5] << endl;
    cout << "7 cards: " << win_rates[6] << " \pm " << win_rate_errors[6] << endl;
    cout << "Total winrate: " << win_rate_total << endl;

    return 0;
}


// int main() {
//     vector<unique_ptr<Card>> deck = readDeckFromFile("decklists/test.txt");

//     int n = 10000;

//     random_device rd;
//     mt19937 generator(rd());

//     vector<int> indices(deck.size());
//     iota(indices.begin(), indices.end(), 0);


//     vector<double> win_rates(7, 0.0);

//     for (int i=0; i<n; i++) {
//         shuffle(indices.begin(), indices.end(), generator);

//         vector<unique_ptr<Card>> hand;
//         for (int j = 0; j < 7; j++) {
//             hand.push_back(deck[indices[j]]->clone());
//         }
//         // for (auto& index : {4, 8, 16, 27, 9, 39, 20}) {
//         //     hand.push_back(deck[index]->clone());
//         // }

//         BoardState state;
//         state.hand = move(hand);

//         vector<BoardState> winning_states;

//         recurse(state, winning_states);

//         if (winning_states.size() == 0) {
//             continue;
//         }

//         // Count number of cards left
//         unsigned long cards_left = 0;
//         for (const auto& state : winning_states) {
//             cards_left = max(state.hand.size(), cards_left);
//         }

//         int cards_used = 6 - cards_left;
//         for (int j = cards_used; j<7; j++) {
//             win_rates[j] += 1./(double)n;
//         }
//     }

//     double win_rate_total = win_rates[6] 
//                   + (1 - win_rates[6]) * win_rates[5] 
//                   + (1 - win_rates[6]) * (1 - win_rates[5]) * win_rates[4] 
//                   + (1 - win_rates[6]) * (1 - win_rates[5]) * (1 - win_rates[4]) * win_rates[3] 
//                   + (1 - win_rates[6]) * (1 - win_rates[5]) * (1 - win_rates[4]) * (1 - win_rates[3]) * win_rates[2] 
//                   + (1 - win_rates[6]) * (1 - win_rates[5]) * (1 - win_rates[4]) * (1 - win_rates[3]) * (1 - win_rates[2]) * win_rates[1] 
//                   + (1 - win_rates[6]) * (1 - win_rates[5]) * (1 - win_rates[4]) * (1 - win_rates[3]) * (1 - win_rates[2]) * (1 - win_rates[1]) * win_rates[0];

//     cout << "win_rates: " << endl;
//     cout << "1 card: " << win_rates[0] << endl;
//     cout << "2 cards: " << win_rates[1] << endl;
//     cout << "3 cards: " << win_rates[2] << endl;
//     cout << "4 cards: " << win_rates[3] << endl;
//     cout << "5 cards: " << win_rates[4] << endl;
//     cout << "6 cards: " << win_rates[5] << endl;
//     cout << "7 cards: " << win_rates[6] << endl;
//     cout << "Total winrate: " << win_rate_total << endl;
   
// }