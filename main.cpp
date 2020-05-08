#include <SFML/Graphics.hpp>
#include <functional>
#include <memory>
#include <cmath>

static const int kWidth = 800;
static const int kHeight = 480;
static const double kTicksPerSecond = 60.;
static const double kSkipTicks = 1000. / kTicksPerSecond;
static const int kMaxSkip = 5;

enum class State {Left, Right, None};
enum class PlayerState {Up, Down, None};

using sf::RenderWindow,
    sf::VideoMode,
    sf::Event,
    sf::CircleShape,
    sf::RectangleShape,
    sf::Color,
    sf::Vector2f,
    sf::Font,
    sf::Text,
    sf::FloatRect,
    sf::Time,
    sf::Clock;

using Update = std::function<void()>;
using Draw = std::function<void(RenderWindow&, double)>;

void loop(RenderWindow& window,
          Update update = nullptr,
          Draw draw = nullptr) {
    sf::Int8 loops = 0;
    double interpolation = 0.;
    Clock clock;
    double nextTick = clock.getElapsedTime().asMilliseconds();
    while (window.isOpen())
    {
        sf::Event event;
        loops = 0;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        while (clock.getElapsedTime().asMilliseconds() > nextTick
               && loops < kMaxSkip) {
            if (update) update();
            nextTick += kSkipTicks;
            ++loops;
        }
        window.clear();
        interpolation = (clock.getElapsedTime().asMilliseconds() + kSkipTicks - nextTick) / kSkipTicks;
        if (draw) draw(window, interpolation);
        window.display();
    }
}

int main() {
    RenderWindow window(sf::VideoMode(kWidth, kHeight), "Main Loop");
    RectangleShape shape(Vector2f{50.f, 50.f});
    Vector2f pos{0.f, 0.f};
    Vector2f vel{0.f, 3.f};
    PlayerState pstate = PlayerState::None;
    Update update = [&](){
        pstate = PlayerState::None;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            float ny = pos.y - vel.y;
            if (ny > 0) {
                pos.y = ny;
            }
            pstate = PlayerState::Up;
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            float ny = pos.y + vel.y + shape.getSize().y;
            if (ny < kHeight) {
                pos.y += vel.y;
            }
            pstate = PlayerState::Down;
        }
    };
    Draw draw = [&](RenderWindow& rw, double i) {
        Vector2f dpos;
        dpos.x = pos.x + (vel.x * i);
        dpos.y = pos.y + (vel.y * i);
        shape.setPosition(dpos);
        rw.draw(shape);
    };
    loop(window, update, draw);
    return 0;
}
