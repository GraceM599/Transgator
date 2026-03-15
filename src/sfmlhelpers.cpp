#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include "sfmlhelpers.h"

// a lot of this has been inspired by and copied from my minesweeper project - Rylee

TextureManager::TextureManager() : prefixbutton(prefixsearch), wordsearchbutton(wordsearch), gobutton(gotexture), searchbarsprite(searchbar){
	if (!prefixsearch.loadFromFile("assets/images/prefixsearch.png"))
    	std::cout << "Failed to load prefixsearch" << std::endl;

	prefixbutton.setTexture(prefixsearch, true);
	prefixbutton.setScale({.5, .5});
	prefixbutton.setPosition({292, 308});

	if (!prefixsearch_pressed.loadFromFile("assets/images/prefixsearch_pressed.png"))
		std::cout << "Failed to load prefixsearch_pressed" << std::endl;

	if (!wordsearch.loadFromFile("assets/images/wordsearch.png"))
		std::cout << "Failed to load wordsearch" << std::endl;

	wordsearchbutton.setTexture(wordsearch, true);
	wordsearchbutton.setScale({.5, .5});
	wordsearchbutton.setPosition({1132, 308});

	if (!wordsearch_pressed.loadFromFile("assets/images/wordsearch_pressed.png"))
		std::cout << "Failed to load wordsearch_pressed" << std::endl;

	if (!searchbar.loadFromFile("assets/images/searchbar.png"))
		std::cout << "Failed to load searchbar" << std::endl;

	searchbarsprite.setTexture(searchbar, true);
	searchbarsprite.setScale({.5, .5});
	searchbarsprite.setPosition({585, 150});

	if (!gotexture.loadFromFile("assets/images/gobutton.png"))
		std::cout << "Failed to load go button" << std::endl;
	
	gobutton.setTexture(gotexture, true);
	gobutton.setScale({.51, .51});
	gobutton.setPosition({1148, 149});
}

void DisplayWindow::setupText(sf::Text &text, std::string message, int size, sf::Color color, std::pair<int, int> pos, bool bold){
	text.setString(message);
	text.setCharacterSize(size);
	text.setFillColor(color);
	text.setPosition({float(pos.first), float(pos.second)});
	if (bold)
		text.setStyle(sf::Text::Bold);
}

DisplayWindow::DisplayWindow() :
	window(sf::VideoMode({1920,1080}), "TransGator", sf::Style::Close),
	title(font),
	prefixdescription(font),
	wholedescription(font),
	inputword(font),
  	trietitle(font),
  	trietime(font),
  	trieresults(font),
  	hashtitle(font),
  	hashtime(font),
  	hashresults(font){

	// load our text font
	if (!font.openFromFile("assets/arialuni.ttf"))
		{
			std::cerr << "Error loading font";
		}

	// set all bools to false
	prefix_on = false;
	whole_on = false;

	configureStaticText();
}

void DisplayWindow::configureStaticText(){
	horizontaldivider.setSize({1839.5, 2});
	horizontaldivider.setFillColor(sf::Color::Black);
	horizontaldivider.setPosition({41.5, 501.9});

	verticaldivider.setSize({2, 465});
	verticaldivider.setFillColor(sf::Color::Black);
	verticaldivider.setPosition({960, 540});

	setupText(title, "TRANSGATOR: ENGLISH TO SPANISH", 80, sf::Color::Black, {220, 40}, true);
	setupText(prefixdescription, "Search for translations\n\n\nfor the five most\n\n\ncommon words starting\n\n\nwith a given prefix.", 20, sf::Color::Black, {50, 330}, false);
	setupText(wholedescription, "Search for a direct\n\n\ntranslation of the\n\n\nentered word.", 20, sf::Color::Black, {1650, 342}, false);
	setupText(trietitle, "Trie Implementation", 70, sf::Color::Black, {140, 520}, true);
	setupText(hashtitle, "Hash Map Implementation", 70, sf::Color::Black, {1015, 520}, true);
	setupText(inputword, "", 35, sf::Color::Black, {630, 190}, false);
	setupText(trietime, "", 20, sf::Color::Black, {25, 600}, false);
	setupText(hashtime, "", 20, sf::Color::Black, {980, 600}, false);
	setupText(trieresults, "", 50, sf::Color::Black, {0, 0}, true);
	setupText(hashresults, "", 50, sf::Color::Black, {0, 0}, true);
	cursor.setSize({2, 38});
	cursor.setFillColor(sf::Color::Black);
}

void DisplayWindow::updateCursor(){
	if (cursorclock.getElapsedTime().asSeconds() >= .75){
        cursorvisible = !cursorvisible;
        cursorclock.restart();
    }

    sf::FloatRect bounds = inputword.getGlobalBounds(); 
    cursor.setPosition({bounds.position.x + bounds.size.x + 3, 191});
}

// signifcant help from my minesweeper project with the name typing and checking - Rylee
void DisplayWindow::updateInputText(sf::Event &event){
	// check if text is being entered
	if (auto textEvent = event.getIf<sf::Event::TextEntered>()){

		char letter = static_cast<char>(textEvent->unicode);

		// checking the character is a valid letter
		if ((letter >= 'A' && letter <= 'Z') || (letter >= 'a' && letter <= 'z')){
			if (input.size() < 26){ // arbitrary number for now until we figure out the max char
				input += letter;
			}
		}
	}

	// check if they're deleting characters or if they press enter 
	else if (auto keyEvent = event.getIf<sf::Event::KeyPressed>()){

		// check for backspace and remove letter
		if (keyEvent->code == sf::Keyboard::Key::Backspace && input.size() > 0){
			input.pop_back();
		}
		
		// check for enter and perform the search
		if (keyEvent->code == sf::Keyboard::Key::Enter && input.size() > 0 && (whole_on || prefix_on)){
			runSearch();
		}
	}
	inputword.setString(input);
}

void DisplayWindow::buttonClick(sf::Event &event){
	if (auto clickevent = event.getIf<sf::Event::MouseButtonPressed>()){

		sf::Vector2f mouseposition(clickevent->position);

		// got help with translating sfml 2.x to 3.0.2 from google and AI
		if (textures.prefixbutton.getGlobalBounds().contains(mouseposition)){
			if (prefix_on){
				prefix_on = false;
				textures.prefixbutton.setTexture(textures.prefixsearch, true);
			}
			else {
				prefix_on = true;
				textures.prefixbutton.setTexture(textures.prefixsearch_pressed, true);
				if (whole_on){
					whole_on = false;
					textures.wordsearchbutton.setTexture(textures.wordsearch, true);
				}
			}
		}

		if (textures.wordsearchbutton.getGlobalBounds().contains(mouseposition)){
			if (whole_on){
				whole_on = false;
				textures.wordsearchbutton.setTexture(textures.wordsearch, true);
			}
			else {
				whole_on = true;
				textures.wordsearchbutton.setTexture(textures.wordsearch_pressed, true);
				if (prefix_on){
					prefix_on = false;
					textures.prefixbutton.setTexture(textures.prefixsearch, true);
				}
			}
		}

		if (textures.gobutton.getGlobalBounds().contains(mouseposition)){
			if ((prefix_on || whole_on) && input.size() > 0)
				runSearch();
		}
	}
}

void DisplayWindow::runSearch(){
	hashresult_string = "";
	trieresult_string = "";
	if (whole_on)
	{
		// hashtime_string = hash.getFunctionTime("word search", input);
		// hashresult_string = hash.search(input);
		hashtime_string = "3.1415927"; // temp value until hash map is working for testing
		hashresult_string = "Resultforhash"; // temp value until hash map is working for testing
		
		// something here for trie time
		// trieresult_string = trie.search(input);
		trietime_string = "0.12941";
		trieresult_string = "trieresult";
	}
	else
	{
		// hashtime_string = hash.getFunctionTime("prefix search", input);
		// auto hashresultvec = hash.prefixSearch(input);

		hashtime_string = "24.2819";
		std::vector<std::tuple<std::string, std::string>> hashresultvec = {
			{"hash", "hola"},
			{"goodbye", "adiós"},
			{"please", "por favor"},
			{"thanks", "gracias"},
			{"water", "agua"}
		};

		for (int i = 0; i < 5; i++){
			hashresult_string = hashresult_string + std::to_string(i+1) + ". " + 
			std::get<0>(hashresultvec[i]) + " - " + std::get<1>(hashresultvec[i]) + "\n\n";
		}
		hashresult_string.erase(hashresult_string.size() - 2);

		// include something here for the trie time
		// auto trieresultvec = trie.prefixSearch(input);

		trietime_string = "0.219";
		std::vector<std::tuple<std::string, std::string>> trieresultvec = {
			{"trie", "gato"},
			{"dog", "perro"},
			{"house", "casa"},
			{"book", "libro"},
			{"food", "comida"}
		};

		for (int i = 0; i < 5; i++){
			trieresult_string = trieresult_string + std::to_string(i+1) + ". " + 
			std::get<0>(trieresultvec[i]) + " - " + std::get<1>(trieresultvec[i]) + "\n\n";
		}
		trieresult_string.erase(trieresult_string.size() - 4);
	}
	updateTrieResults();
	updateHashResults();
}

void DisplayWindow::updateTrieResults(){
	trieresults.setString(trieresult_string);
	trietime.setString("Time taken to complete: " + trietime_string);

	if (whole_on){
		sf::FloatRect bounds = trieresults.getGlobalBounds();
		trieresults.setPosition({480 - bounds.size.x/2.0f, 780});
	}

	else
		trieresults.setPosition({25, 650});

}

void DisplayWindow::updateHashResults(){
	hashresults.setString(hashresult_string);
	hashtime.setString("Time taken to complete: " + hashtime_string);

	if (whole_on){
		sf::FloatRect bounds = hashresults.getGlobalBounds();
		hashresults.setPosition({1440 - bounds.size.x/2.0f, 780});
	}

	else
		hashresults.setPosition({985, 650});
}

void DisplayWindow::drawButtons(){
	window.draw(textures.prefixbutton);
	window.draw(textures.wordsearchbutton);
	window.draw(textures.gobutton);
	window.draw(textures.searchbarsprite);
}

void DisplayWindow::drawText(){
	window.draw(horizontaldivider);
	window.draw(verticaldivider);
	window.draw(title);
	window.draw(prefixdescription);
	window.draw(wholedescription);
	window.draw(inputword);
	window.draw(trietitle);
	window.draw(trietime);
	window.draw(trieresults);
	window.draw(hashtitle);
	window.draw(hashtime);
	window.draw(hashresults);
	if (cursorvisible)
		window.draw(cursor);
}

void DisplayWindow::run(){

	while (window.isOpen())
	{
		window.clear(sf::Color::White);
		drawButtons();
		
		while (auto event = window.pollEvent()){

			if (event->is<sf::Event::Closed>()) {
				window.close();
			}
			updateInputText(*event);
			buttonClick(*event);
		}
		updateCursor();
		drawText();
		window.display();
	}
}