#include "ofApp.h"
#include <cctype>
#include <iomanip>
#include <sstream>

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowTitle("Global Currency Converter - CodeLab II");
	// Expanded canvas to cleanly display all 33 currencies in a grid
	ofSetWindowShape(1000, 780);
	ofBackground(25, 27, 34);

	// API Credentials
	apiKey = "fca_live_SGZCwB3FTgSAnO68yzDS5LQVrj5nw07A38yTyCf9";
	apiBaseUrl = "https://api.freecurrencyapi.com/v1/latest";

	// All 33 official currencies supported by Freecurrencyapi
	supportedCurrencies = {
		"EUR", "USD", "JPY", "BGN", "CZK", "DKK", "GBP", "HUF", "PLN", "RON",
		"SEK", "CHF", "ISK", "NOK", "HRK", "RUB", "TRY", "AUD", "BRL", "CAD",
		"CNY", "HKD", "IDR", "ILS", "INR", "KRW", "MXN", "MYR", "NZD", "PHP",
		"SGD", "THB", "ZAR"
	};

	baseCurrencyIndex = 1; // Default Base: USD
	targetCurrencyIndex = 6; // Default Target: GBP

	amountInput = "100.00";
	isInputFocused = false;
	isErrorState = false;
	isLoading = false;
	statusMessage = "Ready";
	conversionResultString = "---";

	// UI Elements positioning
	amountInputBox.set(50, 110, 260, 36);
	convertButtonBox.set(330, 110, 160, 36);

	// Fetch initial exchange rates
	fetchExchangeRates();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	// Header
	ofSetColor(240, 240, 245);
	ofDrawBitmapString("=== DATA DRIVEN GLOBAL CURRENCY CONVERTER ===", 50, 40);
	ofSetColor(160, 165, 180);
	ofDrawBitmapString("33 Active Currencies | Live rates from Freecurrencyapi.com", 50, 60);

	// Amount Input Box
	ofSetColor(220, 220, 230);
	ofDrawBitmapString("1. Amount:", 50, 100);

	if (isInputFocused) {
		ofSetColor(70, 130, 180);
		ofDrawRectangle(amountInputBox.x - 2, amountInputBox.y - 2, amountInputBox.width + 4, amountInputBox.height + 4);
	}
	ofSetColor(40, 44, 55);
	ofDrawRectangle(amountInputBox);

	ofSetColor(255);
	std::string cursor = (isInputFocused && (ofGetFrameNum() / 30) % 2 == 0) ? "|" : "";
	ofDrawBitmapString(amountInput + cursor, amountInputBox.x + 10, amountInputBox.y + 23);

	// Convert Button
	if (isLoading) {
		ofSetColor(100, 100, 110);
	} else {
		ofSetColor(33, 150, 243);
	}
	ofDrawRectRounded(convertButtonBox, 4);
	ofSetColor(255);
	ofDrawBitmapString(isLoading ? "FETCHING..." : "RECALCULATE", convertButtonBox.x + 28, convertButtonBox.y + 23);

	// Grid Parameters for 33 currencies (11 columns x 3 rows)
	const int cols = 11;
	const float btnW = 75;
	const float btnH = 28;
	const float gapX = 6;
	const float gapY = 6;
	const float startX = 50;

	baseCurrencyButtons.clear();
	targetCurrencyButtons.clear();

	// Section 2: Base Currency Selection Grid
	ofSetColor(220, 220, 230);
	ofDrawBitmapString("2. Select Base Currency (Source):", startX, 180);

	for (size_t i = 0; i < supportedCurrencies.size(); ++i) {
		int c = i % cols;
		int r = i / cols;
		ofRectangle btn(startX + c * (btnW + gapX), 195 + r * (btnH + gapY), btnW, btnH);
		baseCurrencyButtons.push_back(btn);

		if ((int)i == baseCurrencyIndex) {
			ofSetColor(0, 150, 136); // Active teal
		} else {
			ofSetColor(48, 52, 65);
		}
		ofDrawRectRounded(btn, 3);

		ofSetColor(255);
		ofDrawBitmapString(supportedCurrencies[i], btn.x + 24, btn.y + 19);
	}

	// Section 3: Target Currency Selection Grid
	float targetStartY = 330;
	ofSetColor(220, 220, 230);
	ofDrawBitmapString("3. Select Target Currency (Destination):", startX, targetStartY);

	for (size_t i = 0; i < supportedCurrencies.size(); ++i) {
		int c = i % cols;
		int r = i / cols;
		ofRectangle btn(startX + c * (btnW + gapX), targetStartY + 15 + r * (btnH + gapY), btnW, btnH);
		targetCurrencyButtons.push_back(btn);

		if ((int)i == targetCurrencyIndex) {
			ofSetColor(233, 30, 99); // Active pink
		} else {
			ofSetColor(48, 52, 65);
		}
		ofDrawRectRounded(btn, 3);

		ofSetColor(255);
		ofDrawBitmapString(supportedCurrencies[i], btn.x + 24, btn.y + 19);
	}

	// Section 4: Result Output Card
	float resultCardY = 480;
	ofSetColor(18, 20, 25);
	ofDrawRectRounded(startX, resultCardY, 890, 110, 8);

	ofSetColor(160, 165, 180);
	ofDrawBitmapString("Conversion Output:", startX + 25, resultCardY + 30);

	ofSetColor(255, 215, 0);
	ofDrawBitmapString(conversionResultString, startX + 25, resultCardY + 68);

	// Section 5: Status / Error Bar
	if (isErrorState) {
		ofSetColor(244, 67, 54);
		ofDrawBitmapString("[ERROR] " + statusMessage, startX, 625);
	} else {
		ofSetColor(76, 175, 80);
		ofDrawBitmapString("[STATUS] " + statusMessage, startX, 625);
	}

	// Section 6: Instructions / Footer
	ofSetColor(120, 125, 140);
	ofDrawBitmapString("Tip: Click any Base button to re-fetch live rates from API. Click Target to convert instantly.", startX, 660);
	ofDrawBitmapString("Handled: Non-numerical input validation, Network timeout/error, Missing rate fallbacks.", startX, 680);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (!isInputFocused) return;

	if (key == OF_KEY_BACKSPACE) {
		if (!amountInput.empty()) {
			amountInput.pop_back();
		}
	} else if (key == OF_KEY_RETURN) {
		isInputFocused = false;
		calculateConversion();
	} else if ((key >= '0' && key <= '9') || key == '.') {
		if (amountInput.length() < 12) {
			if (key == '.' && amountInput.find('.') != std::string::npos) {
				return;
			}
			amountInput += (char)key;
		}
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	isInputFocused = amountInputBox.inside(x, y);

	// Base currency selection
	for (size_t i = 0; i < baseCurrencyButtons.size(); ++i) {
		if (baseCurrencyButtons[i].inside(x, y)) {
			if (baseCurrencyIndex != (int)i) {
				baseCurrencyIndex = (int)i;
				fetchExchangeRates();
			}
			return;
		}
	}

	// Target currency selection
	for (size_t i = 0; i < targetCurrencyButtons.size(); ++i) {
		if (targetCurrencyButtons[i].inside(x, y)) {
			targetCurrencyIndex = (int)i;
			calculateConversion();
			return;
		}
	}

	// Manual convert button
	if (convertButtonBox.inside(x, y)) {
		calculateConversion();
	}
}

//--------------------------------------------------------------
bool ofApp::isValidNumber(const std::string & input) {
	if (input.empty()) return false;
	int decimalCount = 0;
	for (char c : input) {
		if (c == '.') {
			decimalCount++;
			if (decimalCount > 1) return false;
		} else if (!std::isdigit(c)) {
			return false;
		}
	}
	return true;
}

//--------------------------------------------------------------
void ofApp::fetchExchangeRates() {
	isLoading = true;
	statusMessage = "Requesting live rates from Freecurrencyapi...";
	isErrorState = false;

	std::string baseCode = supportedCurrencies[baseCurrencyIndex];
	std::string endpoint = apiBaseUrl + "?apikey=" + apiKey + "&base_currency=" + baseCode;

	ofHttpResponse response = ofLoadURL(endpoint);

	if (response.status == 200) {
		try {
			ofJson jsonResponse = ofJson::parse(response.data.getText());

			if (jsonResponse.contains("data")) {
				exchangeRates.clear();
				for (auto & item : jsonResponse["data"].items()) {
					exchangeRates[item.key()] = item.value().get<double>();
				}
				statusMessage = "Rates synced successfully for Base " + baseCode;
				isLoading = false;
				calculateConversion();
			} else {
				isErrorState = true;
				statusMessage = "API response structure unexpected: 'data' node missing.";
				isLoading = false;
			}
		} catch (const std::exception & e) {
			isErrorState = true;
			statusMessage = std::string("JSON Parse Error: ") + e.what();
			isLoading = false;
		}
	} else {
		isErrorState = true;
		std::stringstream ss;
		ss << "HTTP Error " << response.status << ": Unable to connect to Freecurrencyapi.";
		statusMessage = ss.str();
		isLoading = false;
	}
}

//--------------------------------------------------------------
void ofApp::calculateConversion() {
	// Non-numerical validation required by brief
	if (!isValidNumber(amountInput)) {
		isErrorState = true;
		statusMessage = "Input validation error: Please enter a valid non-negative number.";
		conversionResultString = "ERROR: Non-numerical input provided.";
		return;
	}

	double inputAmount = 0.0;
	try {
		inputAmount = std::stod(amountInput);
	} catch (...) {
		isErrorState = true;
		statusMessage = "Input out of range or parsing failed.";
		return;
	}

	std::string baseCode = supportedCurrencies[baseCurrencyIndex];
	std::string targetCode = supportedCurrencies[targetCurrencyIndex];

	// Same currency edge case
	if (baseCode == targetCode) {
		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << inputAmount << " " << baseCode
		   << " = " << inputAmount << " " << targetCode << " (1:1 direct ratio)";
		conversionResultString = ss.str();
		isErrorState = false;
		statusMessage = "Conversion complete (1:1 identical currency).";
		return;
	}

	// Rate lookup
	if (exchangeRates.find(targetCode) != exchangeRates.end()) {
		double rate = exchangeRates[targetCode];
		double convertedValue = inputAmount * rate;

		std::stringstream ss;
		ss << std::fixed << std::setprecision(2) << inputAmount << " " << baseCode
		   << " = " << std::setprecision(2) << convertedValue << " " << targetCode
		   << "   [Exchange Rate: 1 " << baseCode << " = " << std::setprecision(4) << rate << " " << targetCode << "]";

		conversionResultString = ss.str();
		isErrorState = false;
		statusMessage = "Conversion successful.";
	} else {
		isErrorState = true;
		statusMessage = "Exchange rate for " + targetCode + " not found in API response.";
		conversionResultString = "Rate data missing.";
	}
}
