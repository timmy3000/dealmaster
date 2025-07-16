# DealMaster
🎲 Advanced Deal or No Deal game in C++ with a CPU player, strategic advisor, and comprehensive statistics tracking. Features optimal decision-making algorithms and cross-platform compatibility.

[![C++](https://img.shields.io/badge/C%2B%2B-11%2B-blue.svg)](https://en.cppreference.com/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Cross--Platform-lightgrey.svg)]()
[![Status](https://img.shields.io/badge/Status-Production--Ready-brightgreen.svg)]()

## 🎯 Features

### 🤖 **Advanced CPU Player**
- **Optimal Strategy**: Uses expected value calculations and risk assessment
- **Adaptive Behavior**: Changes strategy based on game phase (early/mid/late)
- **Smart Advisor**: Provides real-time strategic recommendations to human players
- **Auto-Play Mode**: Computer plays independently to maximize winnings

### 🎮 **Enhanced Gameplay**
- **26 Prize Cases**: From $0.01 to $1,000,000
- **Progressive Bank Offers**: Dynamic offers based on remaining prizes
- **Multiple Game Modes**: Human vs CPU, Auto-play, and Advisory modes
- **Professional UI**: Clean, formatted game display with progress tracking

### 📊 **Statistics & Analytics**
- **Persistent Stats**: Game statistics saved between sessions
- **Performance Tracking**: Win rates, average winnings, best performances
- **Data Visualization**: Detailed statistics display and analysis

### 🛡️ **Robust Architecture**
- **Advanced Error Handling**: Custom exception classes with comprehensive validation
- **Memory Safe**: Smart pointers and modern C++ practices
- **Cross-Platform**: Standard library implementation for maximum compatibility
- **Scalable Design**: Object-oriented architecture with clear separation of concerns

## 🚀 Quick Start

### Prerequisites
- C++ compiler with C++11 support (GCC 4.9+, Clang 3.4+, MSVC 2015+)
- CMake 3.10+ (optional, for build system)

### Installation

1. **Clone the repository**
   ```bash
   git clone https://github.com/timmy3000/dealmaster.git
   cd dealmaster
   ```

2. **Compile the game**
   ```bash
   # Using g++
   g++ -std=c++11 -O2 -Wall -o dealmaster main.cpp
   
   # Using clang++
   clang++ -std=c++11 -O2 -Wall -o dealmaster main.cpp
   
   # Using MSVC (Windows)
   cl /EHsc /std:c++11 main.cpp /Fe:dealmaster.exe
   ```

3. **Run the game**
   ```bash
   ./dealmaster        # Linux/macOS
   dealmaster.exe      # Windows
   ```

## 🎲 How to Play

### Game Modes

1. **Human Player Mode**: Play against the bank with CPU advisory
2. **Computer Auto-Play**: Watch the CPU play optimally
3. **Statistics View**: Track your performance over time

### Gameplay Flow

1. **Choose Your Case**: Select from 26 cases (1-26)
2. **Open Cases**: Reveal prizes in other cases each round
3. **Bank Offers**: Receive offers based on remaining prizes
4. **Decision Time**: Accept the deal or continue playing
5. **CPU Advice**: Get strategic recommendations from the advisor
6. **Final Reveal**: Win your case's prize if you reject all offers

### Strategy Tips

- **Early Game**: Be conservative, offers are typically low
- **Mid Game**: Assess risk vs reward carefully
- **End Game**: Consider the variance in remaining prizes
- **Use CPU Advisor**: The computer calculates optimal moves

## 🏗️ Architecture

### Core Components

```
DealMaster/
├── GameException Classes     # Custom error handling
├── GameStats Structure      # Statistics tracking
├── ComputerPlayer Class     # CPU logic and strategy
├── DealOrNoDealGame Class   # Main game engine
└── GameMenu Class          # User interface
```

### Key Features

- **Expected Value Calculations**: CPU uses mathematical models for decisions
- **Risk Assessment**: Standard deviation and probability analysis
- **Persistent Storage**: Statistics saved to local file
- **Input Validation**: Comprehensive error checking and recovery

## 🔧 Technical Details

### CPU Algorithm

The computer player uses advanced decision-making based on:
- **Expected Value**: Average of all remaining prizes
- **Risk Factor**: Standard deviation analysis
- **Probability Assessment**: Chance of beating current offer
- **Game Phase Strategy**: Different approaches for early/mid/late game

### Error Handling

- **Custom Exceptions**: `GameException`, `InvalidInputException`, `GameStateException`
- **Input Validation**: Robust checking for all user inputs
- **File I/O Safety**: Protected file operations with fallback
- **Memory Management**: Smart pointers prevent memory leaks

## 📈 Performance

- **Startup Time**: < 100ms
- **Memory Usage**: < 10MB
- **CPU Usage**: Minimal, optimized algorithms
- **File I/O**: Efficient statistics persistence

## 🤝 Contributing

We welcome contributions! Here's how to help:

1. **Fork the repository**
2. **Create a feature branch** (`git checkout -b feature/amazing-feature`)
3. **Commit your changes** (`git commit -m 'Add amazing feature'`)
4. **Push to the branch** (`git push origin feature/amazing-feature`)
5. **Open a Pull Request**

### Development Guidelines

- Follow C++ best practices and modern standards
- Include comprehensive error handling
- Add unit tests for new features
- Update documentation for API changes
- Maintain cross-platform compatibility

## 🐛 Bug Reports

Found a bug? Please open an issue with:
- **Description**: Clear description of the problem
- **Steps to Reproduce**: Detailed reproduction steps
- **Expected Behavior**: What should happen
- **System Info**: OS, compiler version, etc.

## 📊 Statistics

The game tracks:
- **Games Played**: Total number of games
- **Win Rate**: Percentage of profitable games
- **Average Winnings**: Mean prize amount
- **Best Performance**: Highest single game winnings
- **Total Earnings**: Cumulative winnings across all games

## 🎯 Future Enhancements

- [ ] **Web Interface**: Browser-based version
- [ ] **Multiplayer Support**: Online multiplayer functionality
- [ ] **Tournament Mode**: Competitive gameplay
- [ ] **Custom Prize Sets**: User-defined prize configurations
- [ ] **Advanced Analytics**: Detailed performance metrics
- [ ] **Sound Effects**: Audio feedback and music
- [ ] **Themes**: Customizable visual themes

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- Inspired by the classic "Deal or No Deal" TV show
- Built with modern C++ standards and best practices
- CPU algorithms based on game theory and statistical analysis
- Community feedback and contributions

## 📞 Support

- **Issues**: Use GitHub Issues for bug reports and feature requests
- **Discussions**: Use GitHub Discussions for general questions
- **Email**: [matsuehatim@gmail.com](mailto:matsuehatim@gmail.com)

---

**Made with ❤️ and C++** | **Star ⭐ if you enjoyed the game!**
