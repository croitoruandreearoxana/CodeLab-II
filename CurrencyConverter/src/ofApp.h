#pragma once

#include "ofMain.h"
#include <map>
#include <string>
#include <vector>

class ofApp : public ofBaseApp {
public:
	void setup() override;
	void update() override;
	void draw() override;

	void keyPressed(int key) override;
	void mousePressed(int x, int y, int button) override;

private:
	void fetchExchangeRates();
	void calculateConversion();
	bool isValidNumber(const std::string & input);

	// API Configuration
	std::string apiKey;
	std::string apiBaseUrl;

	// Full 33 Currencies supported by Freecurrencyapi
	std::vector<std::string> supportedCurrencies;
	int baseCurrencyIndex;
	int targetCurrencyIndex;
	std::map<std::string, double> exchangeRates;

	// User input state
	std::string amountInput;
	bool isInputFocused;

	// Output and status tracking
	std::string conversionResultString;
	std::string statusMessage;
	bool isErrorState;
	bool isLoading;

	// UI interactive layout boundaries
	ofRectangle amountInputBox;
	ofRectangle convertButtonBox;
	std::vector<ofRectangle> baseCurrencyButtons;
	std::vector<ofRectangle> targetCurrencyButtons;
};
