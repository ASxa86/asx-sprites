#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <algorithm>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <random>
#include <sstream>
#include <vector>

sf::VertexArray BatchSprites(const std::vector<sf::Sprite>& x)
{
	sf::VertexArray v{sf::PrimitiveType::Quads, x.size() * 4};

	for(auto i = 0; i < x.size(); i++)
	{
		auto& sprite = x[i];
		auto bounds = sprite.getLocalBounds();
		auto& pos = sprite.getPosition();

		// Shift the bounding box by position.
		const auto left = bounds.left;
		const auto top = bounds.top;
		const auto right = bounds.width + bounds.left;
		const auto bottom = bounds.height + bounds.top;

		const auto x = pos.x + bounds.left;
		const auto y = pos.y + bounds.top;
		const auto w = pos.x + bounds.width;
		const auto h = pos.y + bounds.height;

		auto i0 = i * 4 + 0;
		auto i1 = i * 4 + 1;
		auto i2 = i * 4 + 2;
		auto i3 = i * 4 + 3;
		v[i0].position = sf::Vector2f{x, y};
		v[i1].position = sf::Vector2f{x, h};
		v[i2].position = sf::Vector2f{w, h};
		v[i3].position = sf::Vector2f{w, y};

		v[i0].color = sprite.getColor();
		v[i1].color = sprite.getColor();
		v[i2].color = sprite.getColor();
		v[i3].color = sprite.getColor();

		v[i0].texCoords = sf::Vector2f{left, top};
		v[i1].texCoords = sf::Vector2f{left, bottom};
		v[i2].texCoords = sf::Vector2f{right, bottom};
		v[i3].texCoords = sf::Vector2f{right, top};
	}

	return v;
}

int main()
{
	std::random_device rd;						  // obtain a random number from hardware
	std::mt19937 gen(rd());						  // seed the generator
	std::uniform_int_distribution<> distr(-5, 5); // define the range

	using seconds = std::chrono::duration<double>;
	seconds elapsed{};
	std::vector<seconds> frames;
	frames.reserve(1'000'000);

	sf::RenderWindow window{sf::VideoMode{1920, 1080}, "ASX Sprites"};
	window.setFramerateLimit(0);

	// FPS Counter
	sf::Font font;
	font.loadFromFile(FONT_FILE);
	sf::Text text;
	text.setFont(font);

	sf::Texture texture;
	texture.loadFromFile(SPRITE_FILE);
	sf::Sprite sprite;
	sprite.setTexture(texture);
	sprite.setScale({4.0, 4.0});

	std::vector<sf::Sprite> sprites;

	for(auto i = 0; i < 100000; i++)
	{
		sf::Sprite sprite;
		sprite.setTexture(texture);
		sprite.setScale({4.0, 4.0});
		sprite.setPosition({distr(gen) * 100.0F, distr(gen) * 100.0F});
		sprites.emplace_back(sprite);
	}

	while(window.isOpen() == true)
	{
		sf::Event e{};

		while(window.pollEvent(e) == true)
		{
			switch(e.type)
			{
				case sf::Event::EventType::Closed:
					window.close();
			}
		}

		std::stringstream ss;
		ss << std::setprecision(2) << std::fixed << 1.0 / elapsed.count();
		text.setString("FPS: " + ss.str());

		// Physics
		for(auto& sprite : sprites)
		{
			auto x = distr(gen) / 10.0F;
			auto y = distr(gen) / 10.0F;

			auto pos = sprite.getPosition();
			pos.x += x;
			pos.y += y;

			pos.x = std::max(pos.x, 0.0F);
			pos.y = std::max(pos.y, 0.0F);
			pos.x = std::min(pos.x, (float)window.getSize().x);
			pos.y = std::min(pos.y, (float)window.getSize().y);

			sprite.setPosition(pos);
			// sprite.setRotation(sprite.getRotation() + distr(gen) / 10.0F);
		}

		auto start = std::chrono::steady_clock::now();
		window.clear();

		// for(auto& sprite : sprites)
		//{
		//	window.draw(sprite);
		// }

		auto vertexArray = BatchSprites(sprites);
		sf::RenderStates states;
		states.transform.scale({4.0, 4.0});
		states.texture = &texture;
		window.draw(vertexArray, states);

		window.draw(text);
		window.display();
		elapsed = std::chrono::steady_clock::now() - start;
		frames.push_back(elapsed);
	}

	const auto sum = std::accumulate(std::begin(frames), std::end(frames), seconds::zero());
	const auto avg = sum / frames.size();

	std::cout << "Total Frames: " << frames.size() << std::endl;
	std::cout << "AVG INV: " << avg.count() << std::endl;
	std::cout << "AVG FPS: " << 1.0 / avg.count() << std::endl;
	return 0;
}
