#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace sf;


float offsetX=0, offsetY=0;


const int H = 30;
const int W = 82;


String TileMap[H] = {

        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
        "B                         G                                                      B",
        "B                         G                                                      B",
        "B                         G                     GGGGGGG                          B",
        "B                         GGGGGGG      GGGGGGGGG       G                         B",
        "B                                                        G            GG  G      B",
        "B                                                          G   GGGG       G      B",
        "B   G     GGG                                       G                     GG    GB",
        "BG               GGG     GGG                        G                            B",
        "B                             GGG                   G                            B",
        "B                                                   G                          GGB",
        "B                                      GGGG         G                            B",
        "BG                                        G         G            G               B",
        "B                                         G         G                         GGGB",
        "BGGGGG   GGG     G  G        G            G         GGG                          B",
        "B                                         G         G                   GG       B",
        "B                              G          G        GG       G                    B",
        "B                                         G         G                            B",
        "B                     GGGG                GG        G                            B",
        "B                                         G         G      G                     B",
        "B                                  G      G         G                            B",
        "B                                         G       GGGGG                          B",
        "B                              G          G         G                G           B",
        "B                  GGG   G                GG        G    GGG    GG       GG      B",
        "BGGGGG                                    G         G                            B",
        "B    G       GG                           G         G                          GGB",
        "B           GG                            GG                                     B",
        "B          GG                             GGG                                    B",
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",

};

class PLAYER {

public:

    float dx,dy;    // speed
    FloatRect rect;     // (x, y, width, height)
    bool onGround;    // Are we on the ground?
    Sprite sprite;    // upload picture
    float currentFrame;

    PLAYER(Texture &image)    // class constructor
    {
        sprite.setTexture(image);
        rect = FloatRect(7*32,9*32,40,50);    // Initial coordinates

        dx=dy=0.1;
        currentFrame = 0;
    }


    void update(float time)
    {

        rect.left += dx * time;    //x coordinate
        Collision(0);

        //0.0005 This is the acceleration
        //speed = acceleration * time
        if (!onGround) dy=dy+0.0005*time;    // if we are not on the ground, then we fall with acceleration
        rect.top += dy*time;    //y coordinate    //speed is involved in the movement of the character along y
        onGround=false;
        Collision(1);


        currentFrame += 0.005*time;      // animation speed
        if (currentFrame > 6) currentFrame -=6 ;   // animation 6 frames

        // the texture square shifts each time by 48
        // this way we loop through all the animations to the right
        if (dx>0) sprite.setTextureRect(IntRect(48*int(currentFrame),0,35,50));

        // the texture square shifts each time by 48
        // this way we loop through all the animations to the left
        //due to the fact that we took a minus rectWidth, the picture is mirrored
        if (dx<0) sprite.setTextureRect(IntRect(48*int(currentFrame)+35,0,-35,50));


        sprite.setPosition(rect.left - offsetX, rect.top - offsetY);

        dx=0;
    }

    void Collision(int dir)
    {
        // the loop goes through the right tiles
        for (int i = rect.top/32 ; i<(rect.top+rect.height)/32; i++)    // output by height
            // divide the x-coordinate by the thickness of the tile
            for (int j = rect.left/32; j<(rect.left+rect.width)/32; j++)     // output by width
            {
                // if one of these tiles is a wall, then:
                if ((TileMap[i][j]=='B') || (TileMap[i][j]=='G'))
                {
                    if ((dx>0) && (dir==0)) rect.left =  j*32 -  rect.width;    // move to the left if the tile was on the right
                    if ((dx<0) && (dir==0)) rect.left =  j*32 + 32;    // move to the right if the tile was on the left
                    if ((dy>0) && (dir==1))  { rect.top =   i*32 -  rect.height;  dy=0;   onGround=true; }  // move to the left if the tile was on the right
                    if ((dy<0) && (dir==1))  { rect.top = i*32 + 32;   dy=0;}    // move to the right if the tile was on the left
                }

                if (TileMap[i][j]=='0')    //interaction with green squares
                {
                    TileMap[i][j]=' ';
                }

            }

    }
};

int main()
{
    RenderWindow window( VideoMode(600, 400), "Test!");

    Texture t;
    t.loadFromFile("Biker_run.png");   //loading image from file

    Texture tileSet;
    tileSet.loadFromFile("TX Tileset Ground.png");
    Sprite blockSprite(tileSet);
    blockSprite.setTexture(tileSet);

    Texture background;
    background.loadFromFile("background1.png");

    Sprite backgroundSp(background);
/*
    Text text;
    text.setString("Hello world");
    text.setCharacterSize(24); // in pixels, not points!

// set the color
    text.setFillColor(sf::Color::Red);

// set the text style
    text.setStyle(Text::Bold | Text::Underlined);
    text.setPosition(0, 0);


// inside the main loop, between window.clear() and window.display()
    window.draw(text);
    */
    /*
    Texture textureBackground;
    textureBackground.loadFromFile("country-platform-preview.png");
    //Background Block
    Sprite spriteBackground;
    spriteBackground.setTexture(textureBackground);
    spriteBackground.setPosition(0, 100);
    */


    float currentFrame=0;    // current frame number

    PLAYER p(t);    // Loading our texture into the player

    /*
    Music music;
    music.openFromFile("cyberpunk-street.ogg");
    music.play();
    */
    Clock clock;

    RectangleShape rectangle( Vector2f(32,32));    //display tiles 32 x 32

    while (window.isOpen())    // Main program loop
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        time = time / 700;    // Speed control

        if (time > 20) time = 20;

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)    // Handling the window close event
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            p.dx = -0.1;    // when you click to the left, we give the player acceleration

        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            p.dx = 0.1;    // when you click to the right, we give the player acceleration
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
            // if we are on the ground only then we can jump and give acceleration -0.4
        {
            if (p.onGround) {
                p.dy = -0.4;
                p.onGround = false;
            }
        }

        p.update(time);

        // bind the camera to the character's coordinates
        // the condition is made so that the left wall does not move
        if (p.rect.left > 300)
            offsetX = p.rect.left -
                      300;    // to get the character in the middle of the screen subtract half the width of the screen
        offsetY = p.rect.top -
                  200;    // to get the character in the middle of the screen subtract half the height of the screen

        // window.clear(Color::White);     // Clearing the screen
        window.draw(backgroundSp);

        int coin = 0;
        for (int i = 0; i < H; i++) {   // output by height
            for (int j = 0; j < W; j++)    // output by width
            {
                if (TileMap[i][j] == 'G') {
                    blockSprite.setTextureRect(IntRect(64, 256, 40, 40));
                    blockSprite.setPosition(j * 32 - offsetX, i * 32 - offsetY);
                    window.draw(blockSprite);
                }

                if (TileMap[i][j] == 'B') {
                    rectangle.setFillColor(Color::Black);   // If there is a letter b, paint it black


                    rectangle.setPosition(j * 32 - offsetX, i * 32 - offsetY);    // when we draw a map we shift
                    window.draw(rectangle);
                }
            }
        }

        //window.draw(spriteBackground);
        window.draw(p.sprite);
        window.draw(rectangle);
        window.display();   // Output on display

    }

    return 0;
}



