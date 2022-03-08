#include<stdlib.h>
#include<time.h>
#include<iostream>
#include<string>
#include<SFML/Window.hpp>
#include<SFML/Graphics.hpp>
using namespace std;
using namespace sf;
const int mazeBox=600;
int w=60;
int frameRate=15;
int rows=mazeBox/w;
int columns=rows;
int cs=32;
int level=1;
int characterX,characterY,character1X=columns-1,character1Y=rows-1;
bool key1=false,key2=false;
int currentI=0,currentJ=0;
int i=0,j=0;
bool Left=false,Right=false,up=false,down=false,Left1=false,Right1=false,up1=false,down1=false;
int kCount=0,k1Count=0;
bool mazeGenerated=false;
bool multiplayer=false;
bool gameOver=false;
bool btnclicked=false;
bool playclicked=false,singleclicked=false,multiclicked=false,easyclicked=false,mediumclicked=false,hardclicked=false,reclicked=false,mainclicked=false;
bool clockcheck=true;
int menu=1;
int timestore;
int levelTime=40;
int loadingCount=1;
Texture t,t1,bg;
Sprite s,s1;
Text text,showtime,winner;
class cell{
    bool wall[4];
    bool visited;
public:
    int i,j;
    cell(){
    }
    cell(int x,int y){
        wall[0]=true;
        wall[1]=true;
        wall[2]=true;
        wall[3]=true;
        visited=false;
        i=x;
        j=y;
    }
    void draw(RenderWindow &game){
        RectangleShape linex(Vector2f(w,2));
        RectangleShape line(Vector2f(600,2));
        RectangleShape liney(Vector2f(2,w));
        linex.setFillColor(Color(0,0,150));
        liney.setFillColor(Color(0,0,150));
        line.setPosition(0,598);
        line.setFillColor(Color(0,0,150));
        game.draw(line);
        //fillcolor in visited cell

        if(visited==true)
        {
           /*RectangleShape rect(Vector2f(w,w));
            rect.setPosition(i*w,j*w);
            rect.setFillColor(Color(255,255,255));
            game.draw(rect);*/
        }
        //top wall
        if(wall[0]){
            linex.setPosition(i*w,j*w);
            game.draw(linex);
        }
        //bottom wall
        if(wall[1]){
        linex.setPosition(i*w,j*w+w);
        game.draw(linex);
        }

        //left wall
        if(wall[2]){
        liney.setPosition(i*w,j*w);
        game.draw(liney);
        }
        //right wall
        if(wall[3]){
        liney.setPosition((i*w+w),j*w);
        game.draw(liney);
        }


    }
    void setTopWall(bool t)
    {
        wall[0]=t;
    }
    void setBottomWall(bool b){
        wall[1]=b;
    }
    void setLeftWall(bool l){
        wall[2]=l;
    }
    void setRightWall(bool r){
        wall[3]=r;
    }
    void setVisited(bool c)
    {
        visited=c;
    }
    bool getTopwall(){
        return wall[0];
    }
    bool getBottomwall(){
        return wall[1];
    }
    bool getLeftwall(){
        return wall[2];
    }
    bool getRightwall(){
        return wall[3];
    }
    bool checkVisited()
    {
        return visited;
    }
    void reset(int x,int y)
    {
        wall[0]=true;
        wall[1]=true;
        wall[2]=true;
        wall[3]=true;
        visited=false;
        i=x;
        j=y;
    }
    bool operator==(cell c1)
    {
        if(i==c1.i&&j==c1.j)
        {
            return true;
        }
        return false;
    }
};

class stack{
    int length;
    cell stackCells[3600];
public:
    stack(){
        length=-1;
    }
    void push(cell c)
    {
        if(length<3600){
            length++;
            stackCells[length]=c;
        }
    }
    cell pop()
    {
        if(length>=0)
        {
            length--;
            return stackCells[length];
        }
    }
    int getLength()
    {
        return length;
    }
    void reset()
    {
        length=-1;
    }
};

int index(int a)
{
    if(a<0||a>rows-1)
    {
        return -1;
    }
    return 0;
}
void loadCharacter1()
{
    if(level==7){
            t.loadFromFile("images/level7.png");
            t.setSmooth(true);
            s.setTexture(t);
    }
    if(level==9){
        t.loadFromFile("images/level9.png");
        t.setSmooth(true);
        s.setTexture(t);
    }
    else{
        t.loadFromFile("images/level1.png");
        t.setSmooth(true);
        s.setTexture(t);
    }
    s.setPosition(((w-cs)/2),w-cs);
    s.setTextureRect(IntRect(0*cs,0*cs,cs,cs));
}
void loadCharacter2()
{
    if(level==7){
            t1.loadFromFile("images/level7.png");
            t1.setSmooth(true);
            s1.setTexture(t);
    }
    if(level==9){
        t1.loadFromFile("images/level9.png");
        t1.setSmooth(true);
        s1.setTexture(t);
    }
    else{
        t1.loadFromFile("images/level1.png");
        t1.setSmooth(true);
        s1.setTexture(t);
    }
    s1.setTextureRect(IntRect(3*cs,0*cs,cs,cs));
    s1.setPosition((columns-1)*w+((w-cs)/2),(rows-1)*w+(w-cs));
}
void loadgame(RenderWindow &game ,cell cells[600][600],stack &st)
{
    if(multiclicked) multiplayer=true;
    if(singleclicked) multiplayer=false;
    btnclicked=true;
    if(singleclicked)
    {
        levelTime=40;
        w=60;
        cs=32;
        level=1;
        frameRate=15;
        rows=mazeBox/w;
        columns=rows;
        currentI=rows-1;
        currentJ=columns-1;
        loadCharacter1();
    }
    else if(multiclicked){
        if(easyclicked)
        {
            levelTime=120;
            w=60;
            cs=32;
            level=1;
            frameRate=15;
        }
        else if(mediumclicked)
        {
            levelTime=180;
            w=40;
            cs=32;
            level=5;
            frameRate=15;
        }
        else if(hardclicked)
        {
            levelTime=300;
            w=20;
            cs=20;
            level=9;
            frameRate=20;
        }
        rows=mazeBox/w;
        columns=rows;
        currentI=rows/2;
        currentJ=columns/2;
        loadCharacter1();
        loadCharacter2();
    }
    for(j=0;j<rows;j++){
           for(i=0;i<columns;i++){
                cells[i][j].reset(i,j);
                }

    }
    srand(time(NULL));
    key1=false;
    key2=false;
    cells[currentI][currentJ].setVisited(true);
    st.reset();
    st.push(cells[currentI][currentJ]);
    characterX=0;
    characterY=0;
    character1X=rows-1;
    character1Y=columns-1;
    mazeGenerated=false;
    s.setPosition((w-cs)/2,w-cs);
    s.setTextureRect(IntRect(0*cs,0*cs,cs,cs));
    s1.setPosition((rows-1)*w+(w-cs)/2,(rows-1)*w+w-cs);
    s1.setTextureRect(IntRect(3*cs,0*cs,cs,cs));
    text.setString("");
    gameOver=false;
    game.draw(text);
    game.setFramerateLimit(0);
}
void won(RenderWindow &game,int player)
{
    gameOver=true;
    clockcheck=false;
    RectangleShape winbox(Vector2f(400,200));
    winbox.setPosition(120,200);
    winbox.setFillColor(Color(200,0,0,200));
    game.draw(winbox);
    if(player==0){
        winner.setPosition(250,250);
        winner.setCharacterSize(60);
        winner.setString("Draw");
        winner.setColor(Color::Green);
        game.draw(winner);
    }
    else{
        winner.setPosition(130,250);
        winner.setCharacterSize(60);
        winner.setString("Player "+to_string(player)+" Won");
        winner.setColor(Color::Green);
    }
    game.draw(winner);
}
int main()
{
    RenderWindow game(VideoMode(900,600),"Maze Runner",Style::Close);
    cell cells[600][600],neighbor[4];
    bool showhelp=false;
    stack st;
    Clock clock;
    Text loading,hometext,replaytext,exittext;
    Time ms=milliseconds(200),timecount;
    Texture gameover;
    Texture btn;
    btn.loadFromFile("images/btn1.png");
    Sprite replay,mainMenu,exit;
    replay.setTexture(btn);
    mainMenu.setTexture(btn);
    exit.setTexture(btn);
    gameover.loadFromFile("images/gameover.png");
    Sprite gameoverPic(gameover);
    for(j=0;j<600;j++){
           for(i=0;i<600;i++){
                cell c(i,j);
                cells[i][j]=c;
                }
    }
    RectangleShape helpbox(Vector2f(700,500));
    helpbox.setPosition(100,-405);
    //make rectangle for score,time,etc.
    RectangleShape sidebar(Vector2f(250,200));
    sidebar.setPosition(625,10);
    sidebar.setFillColor(Color(0, 0, 0));
    sidebar.setOutlineColor(Color(0, 102, 0));
    sidebar.setOutlineThickness(9);
    Font a,b,c;
    c.loadFromFile("font/c.ttf");
    a.loadFromFile("font/a.ttf");
    text.setPosition(690,40);
    text.setFillColor(Color::Blue);
    text.setCharacterSize(40);
    text.setFont(a);
    b.loadFromFile("font/b.ttf");
    showtime.setPosition(690,90);
    showtime.setFillColor(Color::Green);
    showtime.setCharacterSize(40);
    showtime.setFont(a);
    winner.setFont(b);
    //loop for running game
    srand(time(NULL));
    while(game.isOpen()){
        Event event;
        while (game.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if (event.type == Event::Closed){
                game.close();
                break;
            }

        }

        game.clear();
        if(btnclicked){
         //variable for counting neighbor
        int c=0;
        //check neighbor around current cell
        if(index(currentJ-1)!=-1){
            if(cells[currentI][currentJ-1].checkVisited()==false){
                neighbor[c]=cells[currentI][currentJ-1];
                c++;
            }
        }
        if(index(currentJ+1)!=-1){
            if(cells[currentI][currentJ+1].checkVisited()==false){
                    neighbor[c]=cells[currentI][currentJ+1];
                    c++;
            }
        }
        if(index(currentI-1)!=-1){
            if(cells[currentI-1][currentJ].checkVisited()==false){
                neighbor[c]=cells[currentI-1][currentJ];
                c++;
            }
        }
        if(index(currentI+1)!=-1){
            if(cells[currentI+1][currentJ].checkVisited()==false){
                    neighbor[c]=cells[currentI+1][currentJ];
                    c++;
            }
        }
        //If the neighbor are >0
        if(c>0)
        {
            //generate random number to choose neighbor
            int r=rand()%c;
            //remove ball between current cell and next cell
            if(index(currentJ-1)!=-1){
                if(cells[currentI][currentJ-1]==neighbor[r]){
                    cells[currentI][currentJ-1].setBottomWall(false);
                    cells[currentI][currentJ].setTopWall(false);
                }
            }
            if(index(currentJ+1)!=-1){
                if(cells[currentI][currentJ+1]==neighbor[r]){
                        cells[currentI][currentJ+1].setTopWall(false);
                        cells[currentI][currentJ].setBottomWall(false);
                }
            }
            if(index(currentI-1)!=-1){
                if(cells[currentI-1][currentJ]==neighbor[r]){
                    cells[currentI-1][currentJ].setRightWall(false);
                        cells[currentI][currentJ].setLeftWall(false);
                }
            }
            if(index(currentI+1)!=-1){
                if(cells[currentI+1][currentJ]==neighbor[r]){
                        cells[currentI+1][currentJ].setLeftWall(false);
                        cells[currentI][currentJ].setRightWall(false);
                }
            }
            currentI=neighbor[r].i;
            currentJ=neighbor[r].j;
            cells[currentI][currentJ].setVisited(true);
            st.push(cells[currentI][currentJ]);
        }
        else if(st.getLength()>=0)
        {
            cell c1=st.pop();
            currentI=c1.i;
            currentJ=c1.j;
            clock.restart();
            loading.setCharacterSize(60);
            loading.setFont(a);
            loading.setPosition(350,200);
            if(loadingCount==1)
            {
                loading.setString("Loading.");
            }
            else if(loadingCount==2)
            {
                loading.setString("Loading..");
            }
            else if(loadingCount==3)
            {
                loading.setString("Loading...");
                loadingCount=0;
            }
            loadingCount++;
            game.draw(loading);
        }
        else{
            mazeGenerated=true;
        }
        if(mazeGenerated)
        {
            game.setFramerateLimit(frameRate);
            //Draw cells
            for(j=0;j<rows;j++){
               for(i=0;i<columns;i++){
                    cells[i][j].draw(game);
                    }
            }
            game.draw(sidebar);
            if(!multiplayer){
                text.setPosition(690,40);
                text.setString("Level: "+to_string(level));
            }
            if(multiplayer&&level==1)
            {
                text.setPosition(690,40);
                text.setString("EASY");
            }
            else if(multiplayer&&level==5)
            {
                text.setPosition(690,40);
                text.setString("MEDIUM");
            }
            else if(multiplayer&&level==9)
            {
                text.setPosition(690,40);
                text.setString("HARD");
            }
            if(clockcheck){
                timecount=clock.getElapsedTime();
                timestore=levelTime+1-timecount.asSeconds();
            }
            if(timestore==0)
            {
                if(multiplayer)
                {
                    won(game,0);
                }
                else{
                    gameOver=true;
                    clockcheck=false;
                    gameoverPic.setPosition(130,200);
                    game.draw(gameoverPic);
                }
            }
            showtime.setString("Time: "+to_string(timestore));
            game.draw(showtime);
            game.draw(text);
            mainMenu.setPosition(Vector2f(650,250));
            hometext.setString("HOME");
            hometext.setPosition(700,275);
            hometext.setCharacterSize(40);
            hometext.setFont(b);
            replay.setPosition(Vector2f(650,350));
            replaytext.setString("REPLAY");
            replaytext.setPosition(690,375);
            replaytext.setCharacterSize(40);
            replaytext.setFont(b);
            exit.setPosition(Vector2f(650,450));
            exittext.setString("EXIT");
            exittext.setPosition(710,475);
            exittext.setCharacterSize(40);
            exittext.setFont(b);
            game.draw(mainMenu);
            game.draw(hometext);
            game.draw(replay);
            game.draw(replaytext);
             game.draw(exit);
            game.draw(exittext);
            if (Mouse::isButtonPressed(Mouse::Left)&&menu==4)
                {
                        Vector2i position = Mouse::getPosition(game);
                        if(position.x>650&&position.x<840&&position.y>265&&position.y<328)
                        {
                            mainclicked=true;
                            sleep(ms);
                            clockcheck=true;
                            gameOver=false;
                            btnclicked=false;
                            playclicked=false,singleclicked=false,multiclicked=false,easyclicked=false,mediumclicked=false,hardclicked=false;
                            menu=1;
                        }
                        else if(position.x>650&&position.x<840&&position.y>365&&position.y<428)
                        {
                            sleep(ms);
                            menu=4;
                            clockcheck=true;
                            gameOver=false;
                            loadgame(game,cells,st);
                        }
                        else if(position.x>650&&position.x<840&&position.y>465&&position.y<528)
                        {
                            sleep(ms);
                            game.close();
                        }

                }
            //draw switch 1
            if(!key1)
            {
                Texture s;
                if(level==7||level==8)
                {
                    s.loadFromFile("images/switch7.png");
                    Sprite sw(s);
                    sw.setPosition((w-20)/2,(rows-1)*w+(w-20)/2);
                    game.draw(sw);
                }
                else if(level>=9)
                {
                    s.loadFromFile("images/switch9.png");
                    Sprite sw(s);
                    sw.setPosition((w-15)/2,(rows-1)*w+(w-15)/2);
                    game.draw(sw);
                }
                else{
                    s.loadFromFile("images/switch5.png");
                    Sprite sw(s);
                    sw.setPosition((w-30)/2,(rows-1)*w+(w-30)/2);
                    game.draw(sw);
                }
            }
            if(multiplayer)
            {
                if(!key2){
                    Texture s;
                    if(level==7||level==8)
                    {
                        s.loadFromFile("images/switch7.png");
                        Sprite sw(s);
                        sw.setColor(Color::Red);
                        sw.setPosition((rows-1)*w+(w-20)/2,(w-20)/2);
                        game.draw(sw);
                    }
                    else if(level>=9)
                    {
                        s.loadFromFile("images/switch9.png");
                        Sprite sw(s);
                        sw.setColor(Color::Red);
                        sw.setPosition((rows-1)*w+(w-15)/2,(w-15)/2);
                        game.draw(sw);
                    }
                    else{
                        s.loadFromFile("images/switch5.png");
                        Sprite sw(s);
                        sw.setColor(Color::Red);
                        sw.setPosition((rows-1)*w+(w-30)/2,(w-30)/2);
                        game.draw(sw);
                    }
                }
            }
            if(!(Left||Right||up||down)&&!gameOver){
                if(Keyboard::isKeyPressed(Keyboard::Left))
                {
                    if(index(characterX-1)!=-1){
                            if(!cells[characterX][characterY].getLeftwall())
                            {
                                 Left=true;
                            }
                    }
                }
                else if(Keyboard::isKeyPressed(Keyboard::Down))
                {
                    if(index(characterY+1)!=-1){
                            if(!cells[characterX][characterY].getBottomwall()){
                                    down=true;
                            }
                    }
                }
                else if(Keyboard::isKeyPressed(Keyboard::Right))
                {
                    if(index(characterX+1)!=-1){
                            if(!cells[characterX][characterY].getRightwall()){
                                Right=true;

                            }
                    }
                }
                else if(Keyboard::isKeyPressed(Keyboard::Up))
                {
                    if(index(characterY-1)!=-1){
                            if(!cells[characterX][characterY].getTopwall()){
                                up=true;
                            }
                    }
                }
            }
            if(down==true)
            {
                if(kCount!=3)
                {
                                s.setTextureRect(IntRect(kCount*cs,0*cs,cs,cs));
                                if(kCount==2) s.move(0,w/3+w%3);
                                else s.move(0,w/3);
                                game.draw(s);
                                kCount++;
                }
                else{
                    down=false;
                    kCount=0;
                    characterY++;
                }
            }
            else if(up==true)
            {
                if(kCount!=3)
                {
                                s.setTextureRect(IntRect(kCount*cs,3*cs,cs,cs));
                                if(kCount==2) s.move(0,-w/3-w%3);
                                else s.move(0,-w/3);
                                game.draw(s);
                                kCount++;
                }
                else{
                    up=false;
                    kCount=0;
                    characterY--;
                }
            }
            else if(Left==true)
            {
                if(kCount!=3)
                {
                                s.setTextureRect(IntRect(kCount*cs,1*cs,cs,cs));
                                if(kCount==2) s.move(-w/3-w%3,0);
                                else s.move(-w/3,0);
                                game.draw(s);
                                kCount++;
                }
                else{
                    Left=false;
                    kCount=0;
                    characterX--;
                }
            }
            else if(Right==true)
            {
                if(kCount!=3)
                {
                    s.setTextureRect(IntRect(kCount*cs,2*cs,cs,cs));
                    if(kCount==2) s.move(w/3+w%3,0);
                    else s.move(w/3,0);
                    game.draw(s);
                    kCount++;
                }
                else{
                    Right=false;
                    kCount=0;
                    characterX++;
                }
            }
            if(multiplayer){
                if(!(Left1||Right1||up1||down1)&&!gameOver){
                    if(Keyboard::isKeyPressed(Keyboard::A))
                    {
                        if(index(character1X-1)!=-1){
                                if(!cells[character1X][character1Y].getLeftwall())
                                {
                                     Left1=true;
                                }
                        }
                    }
                    else if(Keyboard::isKeyPressed(Keyboard::S))
                    {
                        if(index(character1Y+1)!=-1){
                                if(!cells[character1X][character1Y].getBottomwall()){
                                        down1=true;
                                }
                        }
                    }
                    else if(Keyboard::isKeyPressed(Keyboard::D))
                    {
                        if(index(character1X+1)!=-1){
                                if(!cells[character1X][character1Y].getRightwall()){
                                    Right1=true;

                                }
                        }
                    }
                    else if(Keyboard::isKeyPressed(Keyboard::W))
                    {
                        if(index(character1Y-1)!=-1){
                                if(!cells[character1X][character1Y].getTopwall()){
                                    up1=true;
                                }
                        }
                    }
                }
                if(down1==true)
                {
                    if(k1Count!=3)
                    {
                        s1.setTextureRect(IntRect((k1Count+3)*cs,0*cs,cs,cs));
                        if(k1Count==2) s1.move(0,w/3+w%3);
                        else s1.move(0,w/3);
                        game.draw(s1);
                        k1Count++;
                    }
                    else{
                        down1=false;
                        k1Count=0;
                        character1Y++;
                    }
                }
                else if(up1==true)
                {
                    if(k1Count!=3)
                    {
                        s1.setTextureRect(IntRect((k1Count+3)*cs,3*cs,cs,cs));
                        if(k1Count==2) s1.move(0,-w/3-w%3);
                        else s1.move(0,-w/3);
                        game.draw(s1);
                        k1Count++;
                    }
                    else{
                        up1=false;
                        k1Count=0;
                        character1Y--;
                    }
                }
                else if(Left1==true)
                {
                    if(k1Count!=3)
                    {
                        s1.setTextureRect(IntRect((k1Count+3)*cs,1*cs,cs,cs));
                        if(k1Count==2) s1.move(-w/3-w%3,0);
                        else s1.move(-w/3,0);
                        game.draw(s1);
                        k1Count++;
                    }
                    else{
                        Left1=false;
                        k1Count=0;
                        character1X--;
                    }
                }
                else if(Right1==true)
                {
                    if(k1Count!=3)
                    {
                        s1.setTextureRect(IntRect((k1Count+3)*cs,2*cs,cs,cs));
                        if(k1Count==2) s1.move(w/3+w%3,0);
                        else s1.move(w/3,0);
                        game.draw(s1);
                        k1Count++;
                    }
                    else{
                        Right1=false;
                        k1Count=0;
                        character1X++;
                    }
                }
                game.draw(s1);
            }

            //winning condition
            if(characterX==rows-1&&characterY==columns-1&&key1)
            {
                if(!multiplayer){
                    if(level%2==0&&w>20){
                    w=w-10;
                    rows=mazeBox/w;
                    columns=rows;
                    }
                    level++;
                    for(j=0;j<rows;j++){
                    for(i=0;i<columns;i++){
                        cells[i][j].reset(i,j);
                        }

                    }
                    currentI=rows-1;
                    currentJ=rows-1;
                    key1=false;
                    cells[currentI][currentJ].setVisited(true);
                    srand(time(NULL));
                    characterX=0;
                    characterY=0;
                    mazeGenerated=false;
                    if(level==3)levelTime=50;
                    if(level==5)levelTime=70;
                    if(level==7){
                        cs=27;
                        t.loadFromFile("images/level7.png");
                        t.setSmooth(true);
                        s.setTexture(t);
                        levelTime=120;
                        frameRate=18;
                    }
                    if(level==9){
                        cs=20;
                        t.loadFromFile("images/level9.png");
                        t.setSmooth(true);
                        s.setTexture(t);
                        levelTime=180;
                        frameRate=20;
                    }
                    s.setPosition((w-cs)/2,w-cs);
                    s.setTextureRect(IntRect(0*cs,0*cs,cs,cs));
                    game.setFramerateLimit(0);
                }
                else{
                    won(game,1);
                }

            }
            if(character1X==0&&character1Y==0&&key2&&multiplayer)
            {
                won(game,2);
            }
            game.draw(s);
            //enable switch
            if(characterX==0&&characterY==rows-1)
            {
                key1=true;
            }
            if(key1)
            {
                RectangleShape rect(Vector2f(w-4,w-4));
                rect.setPosition((rows-1)*w+3,(columns-1)*w+3);
                rect.setFillColor(Color(100,250,100,200));
                game.draw(rect);
            }
            if(character1X==columns-1&&character1Y==0)
            {
                key2=true;
            }
            if(key2)
            {
                RectangleShape rect1(Vector2f(w-4,w-4));
                rect1.setPosition(3,3);
                rect1.setFillColor(Color(100,0,0,200));
                game.draw(rect1);
            }

        }
        }
        else{
            game.setFramerateLimit(50);
            Sprite p,h,a,single,multi,easy,medium,hard;
            Text play,title,help,about,singleText,multiText,easyText,mediumText,hardText;
            title.setString("MAZE RUNNER");
            title.setFont(b);
            title.Bold;
            title.setCharacterSize(70);
            title.setPosition(230,50);
            title.setFillColor(Color::Green);
            if(!playclicked)
            {
            p.setTexture(btn);
            p.setPosition(350,150);
            h.setTexture(btn);
            h.setPosition(350,250);
            play.setString("PLAY");
            play.setPosition(405,173);
            play.setCharacterSize(40);
            play.setFont(b);
            help.setString("HELP");
            help.setPosition(405,273);
            help.setCharacterSize(40);
            help.setFont(b);
            exit.setPosition(350,350);
            exittext.setString("EXIT");
            exittext.setPosition(405,373);
            exittext.setCharacterSize(40);
            exittext.setFont(b);
            game.draw(p);
            game.draw(h);
            game.draw(a);
            game.draw(play);
            game.draw(help);
            game.draw(about);
            game.draw(exit);
            game.draw(exittext);
            }
            game.draw(title);
            if(playclicked)
            {
                single.setTexture(btn);
                single.setPosition(350,150);
                multi.setTexture(btn);
                multi.setPosition(350,250);
                singleText.setString("SinglePlayer");
                singleText.setPosition(365,180);
                singleText.setCharacterSize(28);
                singleText.setFont(b);
                multiText.setString("MultiPlayer");
                multiText.setPosition(363,280);
                multiText.setCharacterSize(30);
                multiText.setFont(b);
                game.draw(single);
                game.draw(multi);
                game.draw(singleText);
                game.draw(multiText);
                if (Mouse::isButtonPressed(Mouse::Left)&&menu==2)
                {
                        Vector2i position = Mouse::getPosition(game);
                        if(position.x>350&&position.x<540&&position.y>165&&position.y<228&&!singleclicked)
                        {
                            singleclicked=true;
                            sleep(ms);
                            menu=4;
                            loadgame(game,cells,st);
                        }
                        else if(position.x>350&&position.x<540&&position.y>265&&position.y<328&&!multiclicked)
                        {
                            multiclicked=true;
                            sleep(ms);
                            menu=3;
                        }
                }
                if(multiclicked){
                        easy.setTexture(btn);
                        easy.setPosition(350,150);
                        medium.setTexture(btn);
                        medium.setPosition(350,250);
                        hard.setTexture(btn);
                        hard.setPosition(350,350);
                        easyText.setString("EASY");
                        easyText.setPosition(405,173);
                        easyText.setCharacterSize(40);
                        easyText.setFont(b);
                        mediumText.setString("MEDIUM");
                        mediumText.setPosition(385,273);
                        mediumText.setCharacterSize(40);
                        mediumText.setFont(b);
                        hardText.setString("HARD");
                        hardText.setPosition(405,373);
                        hardText.setCharacterSize(40);
                        hardText.setFont(b);
                        game.draw(easy);
                        game.draw(medium);
                        game.draw(hard);
                        game.draw(easyText);
                        game.draw(mediumText);
                        game.draw(hardText);
                        if (Mouse::isButtonPressed(Mouse::Left)&&menu==3)
                        {
                            Vector2i position = Mouse::getPosition(game);
                            if(position.x>350&&position.x<540&&position.y>165&&position.y<228&&!easyclicked)
                            {
                                easyclicked=true;
                                sleep(ms);
                                menu=4;
                                loadgame(game,cells,st);
                            }
                            else if(position.x>350&&position.x<540&&position.y>265&&position.y<328&&!mediumclicked)
                            {
                                mediumclicked=true;
                                sleep(ms);
                                menu=4;
                                loadgame(game,cells,st);
                            }
                            else if(position.x>350&&position.x<540&&position.y>365&&position.y<428&&!hardclicked)
                            {
                                hardclicked=true;
                                sleep(ms);
                                menu=4;
                                loadgame(game,cells,st);
                            }
                        }
                }
            }
            if (Mouse::isButtonPressed(Mouse::Left)&&menu==1)
            {
                    Vector2i position = Mouse::getPosition(game);
                    if(position.x>350&&position.x<540&&position.y>165&&position.y<228&&!playclicked)
                    {
                        playclicked=true;
                        sleep(ms);
                        menu=2;
                    }
                    else if(position.x>350&&position.x<540&&position.y>265&&position.y<328)
                    {
                        sleep(ms);
                        showhelp=true;
                        menu=5;

                    }
                    else if(position.x>350&&position.x<540&&position.y>365&&position.y<428)
                    {
                            sleep(ms);
                            game.close();
                    }
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Escape))
        {
            if(showhelp)
            {
                showhelp=false;
                helpbox.setPosition(100,-405);
                menu =1;
            }
        }
        if(showhelp)
        {
            Vector2f helppos=helpbox.getPosition();
            Text t;
            game.draw(helpbox);
            if(helppos.y<=50)
            {
                helpbox.move(0,20);
            }
            else
            {
                t.setFont(a);
                t.setCharacterSize(40);
                t.setFillColor(Color::Red);
                t.setString("HELP");
                t.setPosition(405,50);
                game.draw(t);
                t.setCharacterSize(25);
                t.setPosition(130,80);
                t.setFont(c);
                t.setString("Single Player: \n1- You can move the character using up,down,left,right arrow keys\n2- You have to enable green switch.\n3- After enabling switch a door will open and you have to go there \n\t before time out.\nMultiplayer:\n1- First player can move the character using up,down,left,right \n\tarrow keys.\n2- Second player can move the character using a,s,d,w keys.\n3-First player has to enable green switch and Second player has to \n\tenable red switch.\n4-After enabling green switch a door will open for first player and \nfirst player go there.\n5- After enabling red switch a door will open for second player and\n\tsecond player have to go there.");
                game.draw(t);
            }

        }
        game.display();
    }
    return 0;
}
