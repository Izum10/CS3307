#include "WordleApplication.h"
#include <algorithm>
#include <cctype>
#include <ctime>
#include <fstream> // Include for file handling

WordleApplication::WordleApplication(const Wt::WEnvironment& env)
    : WApplication(env) {
    setTitle("Wordle"); // Set the application title

    // Initialize the word list from the text file
    WordList();

    // Create a text widget for displaying past attempts
    previousT_ = root()->addWidget(std::make_unique<Wt::WText>());
    previousT_->setWordWrap(true);

    // Widgets
    root()->addWidget(std::make_unique<Wt::WText>("Enter Guess, 5 Letters Maximum: "));

    wInput_ = root()->addWidget(std::make_unique<Wt::WLineEdit>());
    wInput_->setMaxLength(5);
    wInput_->setFocus();

    sClick_ = root()->addWidget(std::make_unique<Wt::WPushButton>("Submit"));
    sClick_->clicked().connect(this, &WordleApplication::validate);

    rClick_ = root()->addWidget(std::make_unique<Wt::WPushButton>("Restart"));
    rClick_->clicked().connect(this, &WordleApplication::restartGame);
    rClick_->hide();

    output_ = root()->addWidget(std::make_unique<Wt::WText>());

    // Initialize the game
    initializeGame();
}

void WordleApplication::WordList() {
    std::ifstream wordFile("word_list.txt");
    if (!wordFile.is_open()) {
        // Handle the case where the file cannot be opened
        Wt::log("error") << "Failed to open word_list.txt";
        return;
    }

    std::string word;
    while (std::getline(wordFile, word)) {
        // Remove any trailing newline characters
        word.erase(std::remove(word.begin(), word.end(), '\n'), word.end());
        wordList_.push_back(word);
    }

    wordFile.close();
}

void WordleApplication::initializeGame() {
    // Select a random word from the list as the target word
    srand(static_cast<unsigned int>(time(nullptr)));
    int randomIndex = rand() % wordList_.size();
    targetWord_ = wordList_[randomIndex];

    // Clear previous game data
    attempts_ = 0;
    guessedWord_ = "_____";

    // Update the result text
    output_->setTextFormat(Wt::TextFormat::UnsafeXHTML);
}

bool WordleApplication::isAlpha(const std::string& str) {
    for (char c : str) {
        if (!isalpha(c)) {
            return false;
        }
    }
    return true;
}

void WordleApplication::validate() {
    std::string guess = wInput_->text().toUTF8();
    std::transform(guess.begin(), guess.end(), guess.begin(), ::tolower);
    std::transform(targetWord_.begin(), targetWord_.end(), targetWord_.begin(), ::tolower);

    // Clear the error message
    output_->setText("");

    // Validate the guess to allow only letters
    if (!isAlpha(guess) || guess.length() != 5) {
        output_->setText("<br>Please enter a valid 5-letter word.");
    } else {
        attempts_++;
        if (guess == targetWord_) {
            // Display a message and restart button
            std::string message = "<br>You guessed the word correctly! Play again? ";
            std::string rClick = "<button onclick=\"location.reload();\">Restart</button>";
            output_->setText(message + rClick);
            sClick_->hide(); // Hide submit button
        } else {
            std::string feedback = getFeedback(guess);
            if (attempts_ >= 6) {
                // Display a message and restart button
                std::string message = "<br>You are out of guesses! Play Again? ";
                std::string rClick = "<button onclick=\"location.reload();\">Restart</button>";
                output_->setText(message + rClick);
                sClick_->hide(); // Hide submit button
            }
        }

        // Create the color-coded HTML markup for the current guess
        std::string colorCodedGuess = getFeedback(guess);

        // Add the current guess and its color-coded version to the past attempts
        pastAttempts_.emplace_back("Attempt " + std::to_string(attempts_) + ": " + guess, colorCodedGuess);

        // Update the displayed past attempts
        updateDisplay();
    }
}

void WordleApplication::restartGame() {
    initializeGame(); // Reset the game
    output_->setText(""); // Clear the result text
    sClick_->show(); // Show the submit button
    rClick_->hide(); // Hide restart button
    pastAttempts_.clear(); // Clear past attempts history
    previousT_->setText(""); // Clear displayed past attempts

    // Clear the input field
    wInput_->setText("");
}

void WordleApplication::updateDisplay() {
    std::string attemptsStr;
    for (const auto& attempt : pastAttempts_) {
        std::string displayAttempt = attempt.second;
        std::transform(displayAttempt.begin(), displayAttempt.end(), displayAttempt.begin(), ::toupper);
        attemptsStr += displayAttempt + "<br>";
    }
    previousT_->setTextFormat(Wt::TextFormat::UnsafeXHTML);
    previousT_->setText(attemptsStr);
}

std::string WordleApplication::getFeedback(const std::string& guess) {
    std::string feedback;
    for (int i = 0; i < 5; i++) {
        if (guess[i] == targetWord_[i]) {
            feedback += "<span style='color:green;'>" + std::string(1, guess[i]) + "</span>";
        } else if (targetWord_.find(guess[i]) != std::string::npos) {
            feedback += "<span style='color:blue;'>" + std::string(1, guess[i]) + "</span>";
        } else {
            feedback += "<span style='color:red;'>" + std::string(1, guess[i]) + "</span>";
        }
    }

    return feedback;
}