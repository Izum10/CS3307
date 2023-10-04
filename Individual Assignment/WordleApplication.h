#ifndef WORDLE_APPLICATION_H
#define WORDLE_APPLICATION_H

#include <Wt/WApplication.h>
#include <Wt/WBreak.h>
#include <Wt/WContainerWidget.h>
#include <Wt/WLineEdit.h>
#include <Wt/WPushButton.h>
#include <Wt/WText.h>
#include <Wt/WComboBox.h>
#include <string>
#include <vector>

class WordleApplication : public Wt::WApplication {
public:
    WordleApplication(const Wt::WEnvironment& env);

private:
    // Function to initialize the list of target words from a text file
    void WordList();

    // Function to initialize the game state
    void initializeGame();

    // Function to check if a string consists only of alphabetic characters
    bool isAlpha(const std::string& str);

    // Function to handle the submission of a guess
    void validate();

    // Function to restart the game
    void restartGame();

    // Function to update the display of past attempts
    void updateDisplay();

    // Function to generate color-coded feedback for a guess
    std::string getFeedback(const std::string& guess);

    // Private member variables
    std::vector<std::string> wordList_;
    std::string targetWord_;
    int attempts_;
    std::string guessedWord_;
    Wt::WText* output_;
    Wt::WLineEdit* wInput_;
    Wt::WPushButton* sClick_;
    Wt::WPushButton* rClick_;
    Wt::WText* previousT_;
    std::vector<std::pair<std::string, std::string>> pastAttempts_;
};

#endif // WORDLE_APPLICATION_H
