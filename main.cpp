#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <iomanip>
#include <string>
#include <map>
#include <memory>
#include <chrono>
#include <stdexcept>
#include <limits>
#include <sstream>
#include <fstream>

// Custom exception classes for better error handling
class GameException : public std::exception {
private:
    std::string message;
public:
    GameException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override {
        return message.c_str();
    }
};

class InvalidInputException : public GameException {
public:
    InvalidInputException(const std::string& msg) : GameException("Invalid Input: " + msg) {}
};

class GameStateException : public GameException {
public:
    GameStateException(const std::string& msg) : GameException("Game State Error: " + msg) {}
};

// Game statistics structure
struct GameStats {
    int gamesPlayed = 0;
    int gamesWon = 0;
    double totalWinnings = 0.0;
    double bestWinning = 0.0;
    double averageWinning = 0.0;
    
    void updateStats(double winnings) {
        gamesPlayed++;
        totalWinnings += winnings;
        if (winnings > bestWinning) {
            bestWinning = winnings;
        }
        averageWinning = totalWinnings / gamesPlayed;
        if (winnings > 0) {
            gamesWon++;
        }
    }
    
    void displayStats() const {
        std::cout << "\n=== GAME STATISTICS ===\n";
        std::cout << "Games Played: " << gamesPlayed << std::endl;
        std::cout << "Games Won: " << gamesWon << std::endl;
        std::cout << "Win Rate: " << std::fixed << std::setprecision(1) 
                  << (gamesPlayed > 0 ? (double)gamesWon / gamesPlayed * 100 : 0) << "%" << std::endl;
        std::cout << "Total Winnings: $" << std::fixed << std::setprecision(2) << totalWinnings << std::endl;
        std::cout << "Best Winning: $" << std::fixed << std::setprecision(2) << bestWinning << std::endl;
        std::cout << "Average Winning: $" << std::fixed << std::setprecision(2) << averageWinning << std::endl;
    }
};

// Advanced AI Computer Player
class ComputerPlayer {
private:
    std::mt19937 rng;
    
    // Calculate expected value of remaining cases
    double calculateExpectedValue(const std::vector<double>& remainingPrizes) const {
        if (remainingPrizes.empty()) return 0.0;
        
        double sum = 0.0;
        for (double prize : remainingPrizes) {
            sum += prize;
        }
        return sum / remainingPrizes.size();
    }
    
    // Calculate standard deviation for risk assessment
    double calculateStandardDeviation(const std::vector<double>& remainingPrizes) const {
        if (remainingPrizes.size() <= 1) return 0.0;
        
        double mean = calculateExpectedValue(remainingPrizes);
        double variance = 0.0;
        
        for (double prize : remainingPrizes) {
            variance += (prize - mean) * (prize - mean);
        }
        variance /= remainingPrizes.size();
        
        return std::sqrt(variance);
    }
    
    // Calculate risk-adjusted decision factor
    double calculateRiskFactor(const std::vector<double>& remainingPrizes, double bankOffer) const {
        double expectedValue = calculateExpectedValue(remainingPrizes);
        double stdDev = calculateStandardDeviation(remainingPrizes);
        
        // Risk adjustment based on variance
        double riskAdjustment = stdDev / (expectedValue + 1.0);
        
        // Calculate probability of getting better than bank offer
        int betterCount = 0;
        for (double prize : remainingPrizes) {
            if (prize > bankOffer) betterCount++;
        }
        double probBetter = (double)betterCount / remainingPrizes.size();
        
        return probBetter - riskAdjustment * 0.3; // Conservative approach
    }

public:
    ComputerPlayer() : rng(std::chrono::steady_clock::now().time_since_epoch().count()) {}
    
    // Make optimal decision for computer player
    bool shouldAcceptDeal(const std::vector<double>& remainingPrizes, double bankOffer, int casesRemaining) const {
        if (remainingPrizes.empty()) return true;
        
        double expectedValue = calculateExpectedValue(remainingPrizes);
        
        // Early game strategy (more cases remaining)
        if (casesRemaining > 10) {
            return bankOffer >= expectedValue * 0.9; // Conservative early on
        }
        // Mid game strategy
        else if (casesRemaining > 5) {
            return bankOffer >= expectedValue * 0.85; // More aggressive
        }
        // End game strategy
        else {
            double riskFactor = calculateRiskFactor(remainingPrizes, bankOffer);
            return riskFactor < 0.4 || bankOffer >= expectedValue * 0.8;
        }
    }
    
    // Provide advice to human player
    std::string getAdvice(const std::vector<double>& remainingPrizes, double bankOffer, int casesRemaining) const {
        if (remainingPrizes.empty()) return "Accept the deal!";
        
        double expectedValue = calculateExpectedValue(remainingPrizes);
        double stdDev = calculateStandardDeviation(remainingPrizes);
        double riskFactor = calculateRiskFactor(remainingPrizes, bankOffer);
        
        std::stringstream advice;
        advice << "\n=== AI ADVISOR ===\n";
        advice << "Expected Value: $" << std::fixed << std::setprecision(2) << expectedValue << std::endl;
        advice << "Bank Offer: $" << std::fixed << std::setprecision(2) << bankOffer << std::endl;
        advice << "Offer vs Expected: " << std::fixed << std::setprecision(1) 
               << (bankOffer / expectedValue * 100) << "%" << std::endl;
        advice << "Risk Level: " << std::fixed << std::setprecision(1) 
               << (stdDev / expectedValue * 100) << "%" << std::endl;
        
        if (shouldAcceptDeal(remainingPrizes, bankOffer, casesRemaining)) {
            advice << "RECOMMENDATION: DEAL! The offer is favorable.\n";
        } else {
            advice << "RECOMMENDATION: NO DEAL! You can likely do better.\n";
        }
        
        return advice.str();
    }
    
    // Select cases to open (for computer player)
    std::vector<int> selectCasesToOpen(const std::vector<bool>& casesOpened, int numToOpen) {
        std::vector<int> availableCases;
        for (int i = 0; i < casesOpened.size(); i++) {
            if (!casesOpened[i]) {
                availableCases.push_back(i);
            }
        }
        
        std::shuffle(availableCases.begin(), availableCases.end(), rng);
        
        std::vector<int> selectedCases;
        for (int i = 0; i < std::min(numToOpen, (int)availableCases.size()); i++) {
            selectedCases.push_back(availableCases[i]);
        }
        
        return selectedCases;
    }
};

// Main Game Class
class DealOrNoDealGame {
private:
    std::vector<double> allPrizes;
    std::vector<double> caseValues;
    std::vector<bool> casesOpened;
    std::vector<double> remainingPrizes;
    std::mt19937 rng;
    int playerCase;
    int round;
    double finalWinning;
    GameStats stats;
    std::unique_ptr<ComputerPlayer> aiPlayer;
    
    // Initialize prize values
    void initializePrizes() {
        allPrizes = {
            0.01, 1.0, 5.0, 10.0, 25.0, 50.0, 75.0, 100.0, 200.0, 300.0,
            400.0, 500.0, 750.0, 1000.0, 5000.0, 10000.0, 25000.0, 50000.0,
            75000.0, 100000.0, 200000.0, 300000.0, 400000.0, 500000.0, 750000.0, 1000000.0
        };
        
        if (allPrizes.size() != 26) {
            throw GameStateException("Invalid number of prizes initialized");
        }
    }
    
    // Shuffle and assign prizes to cases
    void shufflePrizes() {
        caseValues = allPrizes;
        std::shuffle(caseValues.begin(), caseValues.end(), rng);
        casesOpened.assign(26, false);
        updateRemainingPrizes();
    }
    
    // Update remaining prizes list
    void updateRemainingPrizes() {
        remainingPrizes.clear();
        for (int i = 0; i < 26; i++) {
            if (!casesOpened[i]) {
                remainingPrizes.push_back(caseValues[i]);
            }
        }
        std::sort(remainingPrizes.rbegin(), remainingPrizes.rend());
    }
    
    // Calculate bank offer
    double calculateBankOffer() const {
        if (remainingPrizes.empty()) return 0.0;
        
        double sum = 0.0;
        for (double prize : remainingPrizes) {
            sum += prize;
        }
        
        double averageValue = sum / remainingPrizes.size();
        
        // Progressive offer percentage based on round
        double offerPercentage = 0.1 + (round * 0.05);
        if (offerPercentage > 0.9) offerPercentage = 0.9;
        
        return averageValue * offerPercentage;
    }
    
    // Display game board
    void displayBoard() const {
        std::cout << "\n=== DEAL OR NO DEAL - ROUND " << round << " ===\n";
        std::cout << "Your Case: " << (playerCase + 1) << std::endl;
        std::cout << "\nCases Status:\n";
        
        for (int i = 0; i < 26; i++) {
            if (i == playerCase) {
                std::cout << "[" << std::setw(2) << (i + 1) << "]";
            } else if (casesOpened[i]) {
                std::cout << " XX ";
            } else {
                std::cout << " " << std::setw(2) << (i + 1) << " ";
            }
            
            if ((i + 1) % 13 == 0) std::cout << std::endl;
        }
        
        std::cout << "\nRemaining Prizes:\n";
        displayRemainingPrizes();
    }
    
    // Display remaining prizes in a formatted way
    void displayRemainingPrizes() const {
        std::cout << "Low Prizes: ";
        for (int i = remainingPrizes.size() - 1; i >= 0; i--) {
            if (remainingPrizes[i] <= 500) {
                std::cout << "$" << std::fixed << std::setprecision(2) << remainingPrizes[i] << " ";
            }
        }
        std::cout << "\n";
        
        std::cout << "High Prizes: ";
        for (double prize : remainingPrizes) {
            if (prize > 500) {
                std::cout << "$" << std::fixed << std::setprecision(0) << prize << " ";
            }
        }
        std::cout << "\n";
    }
    
    // Get valid input from user
    int getValidInput(int min, int max, const std::string& prompt) {
        int input;
        std::string line;
        
        while (true) {
            try {
                std::cout << prompt;
                std::getline(std::cin, line);
                
                if (line.empty()) {
                    throw InvalidInputException("Empty input");
                }
                
                std::stringstream ss(line);
                ss >> input;
                
                if (ss.fail() || !ss.eof()) {
                    throw InvalidInputException("Non-numeric input");
                }
                
                if (input < min || input > max) {
                    throw InvalidInputException("Input out of range (" + 
                                              std::to_string(min) + "-" + std::to_string(max) + ")");
                }
                
                return input;
                
            } catch (const InvalidInputException& e) {
                std::cout << e.what() << ". Please try again.\n";
            }
        }
    }
    
    // Get yes/no input from user
    bool getYesNoInput(const std::string& prompt) {
        std::string input;
        
        while (true) {
            try {
                std::cout << prompt << " (y/n): ";
                std::getline(std::cin, input);
                
                if (input.empty()) {
                    throw InvalidInputException("Empty input");
                }
                
                char ch = std::tolower(input[0]);
                if (ch == 'y') return true;
                if (ch == 'n') return false;
                
                throw InvalidInputException("Invalid choice");
                
            } catch (const InvalidInputException& e) {
                std::cout << e.what() << ". Please enter 'y' or 'n'.\n";
            }
        }
    }
    
    // Open cases selected by player
    void openCases(const std::vector<int>& casesToOpen) {
        std::cout << "\nOpening cases...\n";
        
        for (int caseNum : casesToOpen) {
            if (caseNum < 0 || caseNum >= 26) {
                throw GameStateException("Invalid case number: " + std::to_string(caseNum + 1));
            }
            
            if (casesOpened[caseNum]) {
                throw GameStateException("Case " + std::to_string(caseNum + 1) + " already opened");
            }
            
            casesOpened[caseNum] = true;
            std::cout << "Case " << (caseNum + 1) << " contained: $" 
                      << std::fixed << std::setprecision(2) << caseValues[caseNum] << std::endl;
        }
        
        updateRemainingPrizes();
    }
    
    // Save game statistics to file
    void saveStats() const {
        try {
            std::ofstream file("dealornodeal_stats.txt");
            if (file.is_open()) {
                file << stats.gamesPlayed << std::endl;
                file << stats.gamesWon << std::endl;
                file << stats.totalWinnings << std::endl;
                file << stats.bestWinning << std::endl;
                file.close();
            }
        } catch (const std::exception& e) {
            std::cout << "Warning: Could not save statistics: " << e.what() << std::endl;
        }
    }
    
    // Load game statistics from file
    void loadStats() {
        try {
            std::ifstream file("dealornodeal_stats.txt");
            if (file.is_open()) {
                file >> stats.gamesPlayed;
                file >> stats.gamesWon;
                file >> stats.totalWinnings;
                file >> stats.bestWinning;
                if (stats.gamesPlayed > 0) {
                    stats.averageWinning = stats.totalWinnings / stats.gamesPlayed;
                }
                file.close();
            }
        } catch (const std::exception& e) {
            // If file doesn't exist or is corrupted, start with fresh stats
            stats = GameStats();
        }
    }

public:
    DealOrNoDealGame() : rng(std::chrono::steady_clock::now().time_since_epoch().count()),
                         playerCase(-1), round(0), finalWinning(0.0) {
        try {
            initializePrizes();
            aiPlayer = std::make_unique<ComputerPlayer>();
            loadStats();
        } catch (const std::exception& e) {
            throw GameStateException("Failed to initialize game: " + std::string(e.what()));
        }
    }
    
    ~DealOrNoDealGame() {
        saveStats();
    }
    
    // Main game loop for human player
    void playGame() {
        try {
            std::cout << "Welcome to Deal or No Deal!\n";
            std::cout << "Choose your lucky case (1-26): ";
            
            playerCase = getValidInput(1, 26, "") - 1;
            shufflePrizes();
            round = 1;
            
            std::cout << "\nYou chose case " << (playerCase + 1) << "!\n";
            std::cout << "Now let's see what's in the other cases...\n";
            
            // Game rounds with different number of cases to open
            std::vector<int> casesToOpenPerRound = {6, 5, 4, 3, 2, 1, 1, 1, 1};
            
            for (int roundCases : casesToOpenPerRound) {
                if (remainingPrizes.size() <= 1) break;
                
                displayBoard();
                
                std::cout << "\nSelect " << roundCases << " case(s) to open:\n";
                std::vector<int> casesToOpen;
                
                for (int i = 0; i < roundCases; i++) {
                    int caseChoice;
                    bool validChoice = false;
                    
                    while (!validChoice) {
                        caseChoice = getValidInput(1, 26, "Case " + std::to_string(i + 1) + ": ") - 1;
                        
                        if (caseChoice == playerCase) {
                            std::cout << "You can't open your own case!\n";
                        } else if (casesOpened[caseChoice]) {
                            std::cout << "Case already opened!\n";
                        } else if (std::find(casesToOpen.begin(), casesToOpen.end(), caseChoice) != casesToOpen.end()) {
                            std::cout << "Case already selected for this round!\n";
                        } else {
                            casesToOpen.push_back(caseChoice);
                            validChoice = true;
                        }
                    }
                }
                
                openCases(casesToOpen);
                
                if (remainingPrizes.size() <= 1) break;
                
                // Bank offer
                double bankOffer = calculateBankOffer();
                std::cout << "\n" << std::string(50, '=') << std::endl;
                std::cout << "THE BANK OFFERS: $" << std::fixed << std::setprecision(2) << bankOffer << std::endl;
                std::cout << std::string(50, '=') << std::endl;
                
                // Show AI advice
                std::cout << aiPlayer->getAdvice(remainingPrizes, bankOffer, remainingPrizes.size());
                
                if (getYesNoInput("Deal or No Deal?")) {
                    finalWinning = bankOffer;
                    std::cout << "\nCongratulations! You won $" << std::fixed << std::setprecision(2) 
                              << finalWinning << "!\n";
                    std::cout << "Your case contained: $" << std::fixed << std::setprecision(2) 
                              << caseValues[playerCase] << std::endl;
                    
                    stats.updateStats(finalWinning);
                    return;
                }
                
                round++;
            }
            
            // Final case reveal
            finalWinning = caseValues[playerCase];
            std::cout << "\nNo more deals! You're going home with your case!\n";
            std::cout << "Your case contained: $" << std::fixed << std::setprecision(2) 
                      << finalWinning << "!\n";
            
            stats.updateStats(finalWinning);
            
        } catch (const GameException& e) {
            std::cout << "Game Error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Unexpected Error: " << e.what() << std::endl;
        }
    }
    
    // Computer player auto-play
    void computerPlay() {
        try {
            std::cout << "Computer Player is playing...\n";
            
            // Computer selects a random case
            std::uniform_int_distribution<int> dist(0, 25);
            playerCase = dist(rng);
            shufflePrizes();
            round = 1;
            
            std::cout << "Computer chose case " << (playerCase + 1) << std::endl;
            
            std::vector<int> casesToOpenPerRound = {6, 5, 4, 3, 2, 1, 1, 1, 1};
            
            for (int roundCases : casesToOpenPerRound) {
                if (remainingPrizes.size() <= 1) break;
                
                std::cout << "\n=== ROUND " << round << " ===\n";
                
                // Computer selects cases to open
                std::vector<int> casesToOpen = aiPlayer->selectCasesToOpen(casesOpened, roundCases);
                
                // Remove player's case from selection
                casesToOpen.erase(std::remove(casesToOpen.begin(), casesToOpen.end(), playerCase), casesToOpen.end());
                
                openCases(casesToOpen);
                
                if (remainingPrizes.size() <= 1) break;
                
                double bankOffer = calculateBankOffer();
                std::cout << "\nBank Offer: $" << std::fixed << std::setprecision(2) << bankOffer << std::endl;
                
                // Computer makes decision
                if (aiPlayer->shouldAcceptDeal(remainingPrizes, bankOffer, remainingPrizes.size())) {
                    finalWinning = bankOffer;
                    std::cout << "Computer says: DEAL!\n";
                    std::cout << "Computer won: $" << std::fixed << std::setprecision(2) 
                              << finalWinning << std::endl;
                    std::cout << "Computer's case contained: $" << std::fixed << std::setprecision(2) 
                              << caseValues[playerCase] << std::endl;
                    
                    stats.updateStats(finalWinning);
                    return;
                }
                
                std::cout << "Computer says: NO DEAL!\n";
                round++;
            }
            
            finalWinning = caseValues[playerCase];
            std::cout << "\nComputer's final case contained: $" << std::fixed << std::setprecision(2) 
                      << finalWinning << "!\n";
            
            stats.updateStats(finalWinning);
            
        } catch (const GameException& e) {
            std::cout << "Computer Game Error: " << e.what() << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Unexpected Error: " << e.what() << std::endl;
        }
    }
    
    // Display game statistics
    void displayStatistics() const {
        stats.displayStats();
    }
    
    // Reset statistics
    void resetStatistics() {
        stats = GameStats();
        try {
            std::remove("dealornodeal_stats.txt");
            std::cout << "Statistics reset successfully!\n";
        } catch (const std::exception& e) {
            std::cout << "Warning: Could not delete statistics file: " << e.what() << std::endl;
        }
    }
};

// Main menu system
class GameMenu {
private:
    std::unique_ptr<DealOrNoDealGame> game;
    
public:
    GameMenu() {
        try {
            game = std::make_unique<DealOrNoDealGame>();
        } catch (const std::exception& e) {
            std::cout << "Failed to initialize game: " << e.what() << std::endl;
            throw;
        }
    }
    
    void displayMenu() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "        DEAL OR NO DEAL - MAIN MENU" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Play Game (Human Player)" << std::endl;
        std::cout << "2. Computer Auto-Play" << std::endl;
        std::cout << "3. View Statistics" << std::endl;
        std::cout << "4. Reset Statistics" << std::endl;
        std::cout << "5. Game Rules" << std::endl;
        std::cout << "6. Exit" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }
    
    void displayRules() {
        std::cout << "\n" << std::string(50, '=') << std::endl;
        std::cout << "                 GAME RULES" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
        std::cout << "1. Choose your lucky case (1-26)" << std::endl;
        std::cout << "2. Open other cases to reveal their prizes" << std::endl;
        std::cout << "3. The bank will make offers based on remaining prizes" << std::endl;
        std::cout << "4. Decide: DEAL (accept offer) or NO DEAL (continue)" << std::endl;
        std::cout << "5. If you reject all offers, you win your case's prize" << std::endl;
        std::cout << "6. AI Advisor provides recommendations" << std::endl;
        std::cout << "7. Computer player uses advanced strategy" << std::endl;
        std::cout << "\nPrizes range from $0.01 to $1,000,000" << std::endl;
        std::cout << std::string(50, '=') << std::endl;
    }
    
    void run() {
        int choice;
        
        while (true) {
            try {
                displayMenu();
                
                std::cout << "Enter your choice (1-6): ";
                std::string input;
                std::getline(std::cin, input);
                
                if (input.empty()) {
                    std::cout << "Invalid choice. Please try again.\n";
                    continue;
                }
                
                std::stringstream ss(input);
                ss >> choice;
                
                if (ss.fail() || !ss.eof()) {
                    std::cout << "Invalid choice. Please enter a number.\n";
                    continue;
                }
                
                switch (choice) {
                    case 1:
                        game = std::make_unique<DealOrNoDealGame>();
                        game->playGame();
                        break;
                    case 2:
                        game = std::make_unique<DealOrNoDealGame>();
                        game->computerPlay();
                        break;
                    case 3:
                        game->displayStatistics();
                        break;
                    case 4:
                        game->resetStatistics();
                        break;
                    case 5:
                        displayRules();
                        break;
                    case 6:
                        std::cout << "Thank you for playing Deal or No Deal!\n";
                        return;
                    default:
                        std::cout << "Invalid choice. Please select 1-6.\n";
                }
                
            } catch (const std::exception& e) {
                std::cout << "Error: " << e.what() << std::endl;
                std::cout << "Please try again.\n";
            }
        }
    }
};

// Main function
int main() {
    try {
        GameMenu menu;
        menu.run();
    } catch (const std::exception& e) {
        std::cout << "Fatal Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
