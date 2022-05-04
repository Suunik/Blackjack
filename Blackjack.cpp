#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <windows.h>

// Maximum score before losing.
constexpr int maximumScore{ 21 };

enum class cardRank
{
	cardRank_Ace,
	cardRank_2,
	cardRank_3,
	cardRank_4,
	cardRank_5,
	cardRank_6,
	cardRank_7,
	cardRank_8,
	cardRank_9,
	cardRank_10,
	cardRank_Jack,
	cardRank_Queen,
	cardRank_King,
	max_rank
};

enum class cardSuit
{
	cardSuit_clubs,
	cardSuit_spades,
	cardSuit_hearts,
	cardSuit_diamonds,
	
	
	max_suit
};

class Card
{
private:
	cardRank m_rank{};
	cardSuit m_suit{};
	sf::Sprite m_texture;

public:
	void print() // prindib yksiku kaardi
	{
		switch (m_rank)
		{
		case cardRank::cardRank_2:				std::cout << "2";	break;
		case cardRank::cardRank_3:				std::cout << "3";	break;
		case cardRank::cardRank_4:				std::cout << "4";	break;
		case cardRank::cardRank_5:				std::cout << "5";	break;
		case cardRank::cardRank_6:				std::cout << "6";	break;
		case cardRank::cardRank_7:				std::cout << "7";	break;
		case cardRank::cardRank_8:				std::cout << "8";	break;
		case cardRank::cardRank_9:				std::cout << "9";	break;
		case cardRank::cardRank_10:				std::cout << "10";	break;
		case cardRank::cardRank_Jack:			std::cout << "J";	break;
		case cardRank::cardRank_Queen:			std::cout << "Q";	break;
		case cardRank::cardRank_King:			std::cout << "K";	break;
		case cardRank::cardRank_Ace:			std::cout << "A";	break;
		}
		switch (m_suit)
		{
		case cardSuit::cardSuit_clubs:			std::cout << "C";	break;
		case cardSuit::cardSuit_diamonds:		std::cout << "D";	break;
		case cardSuit::cardSuit_hearts:			std::cout << "H";	break;
		case cardSuit::cardSuit_spades:			std::cout << "S";	break;
		}
	}

	sf::Sprite returnSprite()
	{
		return m_texture;
	}

	int value() // Saadab kaardi v22rtuse
	{
		switch (m_rank)
		{
		case cardRank::cardRank_2:					return 2;
		case cardRank::cardRank_3:					return 3;
		case cardRank::cardRank_4:					return 4;
		case cardRank::cardRank_5:					return 5;
		case cardRank::cardRank_6:					return 6;
		case cardRank::cardRank_7:					return 7;
		case cardRank::cardRank_8:					return 8;
		case cardRank::cardRank_9:					return 9;
		case cardRank::cardRank_10:					return 10;
		case cardRank::cardRank_Jack:				return 10;
		case cardRank::cardRank_Queen:				return 10;
		case cardRank::cardRank_King:				return 10;
		case cardRank::cardRank_Ace:				return 11;
		default:
			return 0;
		}
	}

	Card() = default;

	Card(cardRank rank, cardSuit suit, sf::Sprite sprite)
		: m_rank{ rank }, m_suit{ suit }, m_texture{ sprite }
	{
	}

};

class Deck
{
private:
	std::array < Card, 52 > m_deck;
	int m_cardIndex{0};
	sf::Texture playing_cards;
	

public:
	
	void printDeck() const
	{
		std::cout << '\n';
		for (Card card : m_deck)
		{
			card.print();
			std::cout << ' ';
			static int cardCounter{ 1 };
			if (cardCounter % 13 == 0) // iga 13 kaardi tagant uus rida (Lihtsam lugeda)
			{
				std::cout << '\n';
			}
			++cardCounter;
		}
	}

	void printCard()
	{
		m_deck[m_cardIndex].print();
		std::cout << '\n';
	}
	int returnValue()
	{
		int value{ m_deck[m_cardIndex].value() };
		++m_cardIndex;
		return value;
	}
	void resetIndex()
	{
		m_cardIndex = 0;
	}
	sf::Sprite returnCard(int lampIndex)
	{
		sf::Sprite card{ m_deck[lampIndex].returnSprite() };
		return card;
	}

	void shuffle()
	{
		static std::mt19937 mt{ static_cast<std::mt19937::result_type>(std::time(nullptr)) };

		std::shuffle(m_deck.begin(), m_deck.end(), mt);
	}



	Card dealCard()
	{
		return m_deck[++m_cardIndex];
	}
	Deck()
	{
		int card{ 0 };

		int posX{ 1 }; // position on texture
		int posY{ 1 };

		int width{ 71 }; // card measurements
		int length{ 96 };

		auto suits{ static_cast<int>(cardSuit::max_suit) };
		auto ranks{ static_cast<int>(cardRank::max_rank) };
		

		if (!playing_cards.loadFromFile("playing_cards.png")) // loading texture
		{
		}
		playing_cards.setSmooth(true);


		for (int suit{ 0 }; suit < suits; ++suit)
		{
			for (int rank{ 0 }; rank < ranks; ++rank)
			{
				sf::Sprite cardPicture{}; // Creating Sprite here
				cardPicture.setTexture(playing_cards);
				cardPicture.setTextureRect(sf::IntRect(posX, posY, width, length));
				cardPicture.scale(sf::Vector2f(1.4f, 1.4f));

				Card anotherCard{ m_deck[card] };
				m_deck[card] = { static_cast<cardRank>(rank), static_cast<cardSuit>(suit), static_cast<sf::Sprite>(cardPicture)};

				posX = posX + width + 2; // add card width + 2 pixels to position on Sprite
				++card;
			}
			posX = 1; 
			posY = posY + length +2; // add card lenght +2 pixels to position on Sprite
		}
	}
};

class Player
{
private:
	int m_score{0};
	static int m_cardIndex;
	float m_slotPosition{ 350 };
	int m_arrayCounter{0};
	std::array<float, 52> m_slotPositions{ 350 };
	std::array<int, 52> m_cardIndexs{ 0 };
public:
	void printScore(sf::RenderWindow& window, float posX, float posY)
	{
		std::string scores = std::to_string(m_score);
		sf::Font font;
		if (!font.loadFromFile("times.ttf"))
		{
		}

		sf::Text text;
		text.setFont(font);
		text.setString(scores);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Bold);
		text.setPosition(posX, posY);

		window.draw(text);

	}

	bool isBust()
	{
		return (m_score > maximumScore);
	}

	void reset()
	{
		m_score = 0;
		m_cardIndex = 0;
		m_slotPosition = 350;
		m_arrayCounter = 0;
		m_cardIndexs = { 0 };
	}

	void drawCard(Deck& deck, sf::RenderWindow& window, int i, float x = 150)
	{
		sf::Sprite lampKaart{ deck.returnCard(m_cardIndexs[i]) };
		float position{ m_slotPositions[i] };
		lampKaart.setPosition(sf::Vector2f(position, x));
		window.draw(lampKaart);
	}
	void addToarray()
	{
		m_slotPositions[m_arrayCounter] = m_slotPosition;
		m_cardIndexs[m_arrayCounter] = m_cardIndex;
		++m_arrayCounter;
	}

	void addScore(Deck &deck)
	{
		m_score = m_score + deck.returnValue();
	}

	int returnScore() {return m_score;}
	float returnPosition() const { return m_slotPosition; }
	float addcardPosition() { return m_slotPosition += 120; }
	float setPosition(int x) { return m_slotPosition == x; }
	int addCardindex() { return ++m_cardIndex; }
	int returnIndex() { return m_cardIndex; }
};

class BlackJackTable
{
private:
	std::array<int, 900> m_posX; // for navigating the table
	std::array<int, 750> m_posY;

public:
	bool buttonPressHit(sf::Event event, sf::RenderWindow &window)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);

				if (localPosition.x > m_posX[370] && localPosition.y > m_posY[650])
				{
					if (localPosition.x < m_posX[435] && localPosition.y < m_posY[695])
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	

	bool buttonPressStand(sf::Event event, sf::RenderWindow& window)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);

				if (localPosition.x > m_posX[470] && localPosition.y > m_posY[650])
				{
					if (localPosition.x < m_posX[545] && localPosition.y < m_posY[695])
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	bool buttonPressPlayGame(sf::Event event, sf::RenderWindow& window)
	{
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				sf::Vector2i localPosition = sf::Mouse::getPosition(window);

				if (localPosition.x > m_posX[400] && localPosition.y > m_posY[50])
				{
					if (localPosition.x < m_posX[495] && localPosition.y < m_posY[85])
					{
						return true;
					}
				}
			}
		}
		return false;
	}
	void playGameButton(sf::RenderWindow &window)
	{
		sf::RectangleShape rectangle(sf::Vector2f(95.f, 35.f));
		rectangle.setFillColor(sf::Color::Black);
		rectangle.setOutlineThickness(4.f);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setPosition(400, 50);

		sf::Font font;
		if (!font.loadFromFile("times.ttf"))
		{
		}

		sf::Text text;
		text.setFont(font);
		text.setString("Play game");
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Bold);
		text.setPosition(400, 53);

		window.draw(rectangle);
		window.draw(text);
	}
	void cardSlot(sf::RenderWindow& window, std::string slotName, float posX, float posY)
	{
		sf::RectangleShape rectangle(sf::Vector2f(98.f, 133.f));
		rectangle.setFillColor(sf::Color(7, 99, 36));
		rectangle.setOutlineThickness(3.f);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setPosition(posX, posY);

		sf::Font font;
		if (!font.loadFromFile("times.ttf"))
		{
		}

		sf::Text text;
		text.setFont(font);
		text.setString(slotName);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Bold);
		text.setPosition(posX, (posY + 135));

		window.draw(rectangle);
		window.draw(text);
	}

	void button(sf::RenderWindow& window, std::string slotName, float posX, float posY)
	{
		sf::RectangleShape rectangle(sf::Vector2f(75.f, 35.f));
		rectangle.setFillColor(sf::Color::Black);
		rectangle.setOutlineThickness(4.f);
		rectangle.setOutlineColor(sf::Color::White);
		rectangle.setPosition(posX, posY);

		sf::Font font;
		if (!font.loadFromFile("times.ttf"))
		{
		}

		sf::Text text;
		text.setFont(font);
		text.setString(slotName);
		text.setCharacterSize(20);
		text.setFillColor(sf::Color::White);
		text.setStyle(sf::Text::Bold);
		text.setPosition((posX), (posY + 3));

		window.draw(rectangle);
		window.draw(text);
	}
	
	void youLostScreen(sf::RenderWindow &window)
	{
		sf::Font font;
		if (!font.loadFromFile("times.ttf"))
		{
		}

		sf::Text text;
		text.setFont(font);
		text.setString("YOU LOST!");
		text.setCharacterSize(50);
		text.setFillColor(sf::Color::Red);
		text.setStyle(sf::Text::Bold);
		text.setPosition(340, 350);

		
		window.draw(text);
	}
	void youWonScreen(sf::RenderWindow& window)
	{
		sf::Font font;
		if (!font.loadFromFile("times.ttf"))
		{
		}

		sf::Text text;
		text.setFont(font);
		text.setString("YOU WON!");
		text.setCharacterSize(50);
		text.setFillColor(sf::Color::Green);
		text.setStyle(sf::Text::Bold);
		text.setPosition(340, 350);


		window.draw(text);
	}
	void youTieScreen(sf::RenderWindow& window)
	{
		sf::Font font;
		if (!font.loadFromFile("times.ttf"))
		{
		}

		sf::Text text;
		text.setFont(font);
		text.setString("TIE");
		text.setCharacterSize(50);
		text.setFillColor(sf::Color::Yellow);
		text.setStyle(sf::Text::Bold);
		text.setPosition(410, 350);


		window.draw(text);
	}

	void cardBack(sf::RenderWindow &window,float posX, float posY)
	{
		sf::Texture behindCard; // card back settings
		if (!behindCard.loadFromFile("card_back.png"))
		{
		}
		behindCard.setSmooth(true);


		sf::Sprite card_back;
		card_back.setTexture(behindCard);
		card_back.setTextureRect(sf::IntRect(43, 8, 175, 245));
		card_back.scale(sf::Vector2f(0.58f, 0.55f));
		card_back.setPosition(sf::Vector2f(posX, posY));

		window.draw(card_back);
	}
	void drawTable(sf::RenderWindow& window)
	{
		sf::Texture behindCard; // card back settings
		if (!behindCard.loadFromFile("card_back.png"))
		{
		}
		behindCard.setSmooth(true);


		sf::Sprite card_back;
		card_back.setTexture(behindCard);
		card_back.setTextureRect(sf::IntRect(43, 8, 175, 245));
		card_back.scale(sf::Vector2f(0.58f, 0.55f));
		card_back.setPosition(sf::Vector2f(48.f, 50.f));

		cardSlot(window, "DECK", 50, 50);
		cardBack(window, 50, 50);
		cardSlot(window, "DEALER SCORE:", 350, 150);
		cardSlot(window, "", 470, 150);
		cardSlot(window, "PLAYER SCORE:", 350, 450);
		cardSlot(window, "", 470, 450);

		button(window, "   HIT", 370, 650);
		button(window, "STAND", 470, 650);

	}
	BlackJackTable()
	{
		int number{ 1 };
		for (unsigned int i{ 0 }; i < std::size(m_posX); ++i)
		{
			m_posX[i] = number;
			++number;
		}
		number = 1;
		for (unsigned int i{ 0 }; i < std::size(m_posY); ++i)
		{
			m_posY[i] = number;
			++number;
		}

	}
};

class BlackJack
{
private:
	int m_turn{ 0 };
	int m_stopper{ 0 };
public:


	void drawCardDealer(Player &dealer, Deck& deck, sf::RenderWindow& window, BlackJackTable &blackjacktable)
	{	
		if (m_stopper < 1)
		{
			dealer.addToarray();
			dealer.addCardindex();
			dealer.addcardPosition();
			dealer.addScore(deck);
			++m_stopper;
		}
		for (int i{ 0 }; i < m_stopper; ++i)
		{
			blackjacktable.cardBack(window, 470, 150);
			dealer.drawCard(deck, window, i);
		}
	}

	bool dealerTurn(Player &dealer, Player &player, Deck& deck, sf::RenderWindow& window)
	{
		if (dealer.returnScore() < player.returnScore() || dealer.returnScore() <= 17)
		{
			if (dealer.returnScore() < maximumScore)
			{
				dealer.addToarray();
				dealer.addCardindex();
				dealer.addcardPosition();
				dealer.addScore(deck);
				++m_stopper;
			}
		}

		for (int i{ 0 }; i < m_stopper; ++i)
		{
			dealer.drawCard(deck, window, i);
		}

		return true;
	}
	
	int returnTurn() const {return m_turn;}
	int resetTurn() { return m_turn = 0; }
	int addTurn() { return ++m_turn; }
	void resetStopper() { m_stopper = 0; }
};

int Player::m_cardIndex{ 0 }; // anname static memberile väärtuse

void resetBoard(Player &player, Player &dealer, BlackJack &blackjack, Deck &deck, int &stand)
{
	player.reset(); // resetime kõik memberid
	dealer.reset();
	blackjack.resetStopper();
	deck.resetIndex();
	blackjack.resetTurn();
	stand = 1;
	deck.shuffle();		// segame kaardipaki
}
void hideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

int main()
{

	sf::RenderWindow window(sf::VideoMode(950, 750), "Blackjack"); // Creating window
	window.setFramerateLimit(10);
	hideConsole();

	Deck deck{};		// Teeme kaardipaki

	BlackJackTable blackJackTable{};
	BlackJack blackjack{};

	Player player{};
	Player dealer{};
	
	int gameControl{ 0 };

	int play{ 1 };
	int win{ 2 };
	int tie{ 3 };
	int lose{ -1 };
	int start{ 0 };

	int stand{ 0 };


	while (window.isOpen())
	{

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (gameControl == play)
			{
				if (blackJackTable.buttonPressHit(event, window))	// lisame m_turn, m_cardIndex ja m_slotPosition memberile 
																	//valuet peale igat nupule vajutust
				{
					player.addToarray();		// peab olema esimene!
					player.addCardindex();		// annab uue kaardi indexi
					player.addcardPosition();	// m22rab j2rgmise kaardi asukoha	
					player.addScore(deck);
					blackjack.addTurn();		// annab accessi uuele if statementile peale igat vajutust
				}

				if (blackJackTable.buttonPressStand(event, window))
				{
					stand = 2;
				}
			}
			if (gameControl != play)
			{
				if (blackJackTable.buttonPressPlayGame(event, window))
				{
					resetBoard(player, dealer, blackjack, deck, stand); // resetime kõik muutujad
					gameControl = play;
				}
			}

			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color(7, 99, 36));

		blackJackTable.drawTable(window);

		if (gameControl == start)
		{
			blackJackTable.playGameButton(window);
		}

		if (gameControl == lose)
		{
			blackJackTable.playGameButton(window);
			blackJackTable.youLostScreen(window);
		}

		if (gameControl == win)
		{
			blackJackTable.playGameButton(window);
			blackJackTable.youWonScreen(window);
		}

		if (gameControl == tie)
		{
			blackJackTable.playGameButton(window);
			blackJackTable.youTieScreen(window);
		}
		
		if (stand == 1)
			{
				blackjack.drawCardDealer(dealer, deck, window, blackJackTable);
			}

		if (stand == 2)
		{
			if (dealer.returnScore() > player.returnScore() && dealer.returnScore() <= maximumScore)
			{
					gameControl = lose;
			}
			if (dealer.returnScore() == player.returnScore() && dealer.returnScore() >= 17)
			{
					gameControl = tie;
			}

			if (dealer.returnScore() > maximumScore)
			{
					gameControl = win;
			}

			blackjack.dealerTurn(dealer, player, deck, window);		
		}

		for (int i = 0; i < blackjack.returnTurn(); ++i)
		{
			player.drawCard(deck, window, i, 450);

			if (player.returnScore() > maximumScore)
			{
				gameControl = lose;
			}
		}

		dealer.printScore(window, 525, 285);
		player.printScore(window, 525, 585);



		window.display();

	}
	return 0;
}