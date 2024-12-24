#include<iostream>
#include<windows.h>
#include<mmsystem.h>

using namespace std;

#include "iGraphics.h"
#include "GL.h"
#include <math.h>
#include<string.h>
#include<stdlib.h>
#pragma comment(lib, "winmm.lib") // Alternatively, this works in MSVC
// #pragma comment(lib, "winmm.lib")

#define array_size(x) (sizeof(x)/sizeof((x)[0]))cl

int X_MAX=1366,Y_MAX=768;
double pi= acos(-1);
int basket_x=X_MAX/2,basket_y=160,basket_speed=45;
int hen1x,hen1y;
int egg1x,egg1y;
int point=0;
int resume=0;
int start=1;
int musicOn=1;
//int hen1_poss[60];
int hen1_poss[]={300,250,350,400,450,200,150,100,500,550,420,320,650,720,750,580,570,780,850,900,1000,1100,1200};
int egg1_poss_x[]={300,250,350,400,450,200,150,100,500,550,420,320,650,720,750,580,570,780,850,900,1000,1100,1200};


char pnt[100],s_time[100];

int len=array_size(egg1_poss_x);
//int egg1_poss_y_forward[array_size(egg1_poss_x)];
//int egg1_poss_y_backward[array_size(egg1_poss_x)];
//int egg1_present_forward[array_size(egg1_poss_x)]={0};
//int egg1_present_backward[array_size(egg1_poss_x)]={0};

int egg1_y[array_size(egg1_poss_x)][2];
int type[array_size(hen1_poss)][2]={0};
int present[array_size(egg1_poss_x)][2]={0};
int break_egg[array_size(egg1_poss_x)][2]={0};

int finish=0;
int tm=60,tm_input=60;

int hen1_direction=1;

int dy=13;

void draw_egg(int x,int y);

void hen1_moving();

char hen1[]="hen1.bmp";
char hen2[]="hen2.bmp";
char hen3[]="hen3.bmp";
char hen[]="hen1.bmp";

void instruction_page()
{
    iSetColor(254,184,198);
    iFilledRectangle(0,0,X_MAX,Y_MAX);

    iSetColor(255,85,85);
    iText(125,640,"CHOOSE HEN:",GLUT_BITMAP_TIMES_ROMAN_24);
    iRectangle(300,600,100,100);
    iShowBMP2(300,605,hen1,0xffffff);
    iRectangle(410,600,100,100);
    iShowBMP2(410,605,hen2,0xffffff);
    iRectangle(520,600,100,100);
    iShowBMP2(520,605,hen3,0xffffff);


    iText(125,535,"CHOOSE TIME(seconds):",GLUT_BITMAP_TIMES_ROMAN_24);
    iRectangle(400,535,40,40);
    iText(400,545,"60",GLUT_BITMAP_TIMES_ROMAN_24);
    iRectangle(450,535,40,40);
    iText(452,545,"120",GLUT_BITMAP_TIMES_ROMAN_24);
    iRectangle(500,535,40,40);
    iText(502,545,"180",GLUT_BITMAP_TIMES_ROMAN_24);

    //instruction
    iSetColor(0,0,0);

    iText(125,470,"INSTRUCTIONS:",GLUT_BITMAP_TIMES_ROMAN_24);

    iText(125,440,"(1) EGG : i. Don't catch the poops(-10 points) ",GLUT_BITMAP_HELVETICA_18);
    iText(200,410,"ii. Try to catch the- (A) GOLDEN eggs(+10 points) ",GLUT_BITMAP_HELVETICA_18);
    iText(365,380,"(B) BLUE eggs(+5 points) ",GLUT_BITMAP_HELVETICA_18);
    iText(365,350,"(C) NORMAL eggs(+1 points) ",GLUT_BITMAP_HELVETICA_18);

    //BASKET
    iText(125,300,"(2) BASKET MOVEMENT: (i) Use  LEFT ARROW  KEY to move left ",GLUT_BITMAP_HELVETICA_18);
    iText(340,270,"(ii) Use  RIGHT ARROW KEY  to move right ",GLUT_BITMAP_HELVETICA_18);

    iText(125,230,"(3) Use  F1 KEY  to pause and play",GLUT_BITMAP_HELVETICA_18);

    iText(125,180,"(4) Use the given  NAVIGATION BUTTON  to play  ,  resume  ,  restart  ,  exit  etc.",GLUT_BITMAP_HELVETICA_18);
    iText(125,130,"(5) Click the  LET'S START  button to start the game",GLUT_BITMAP_HELVETICA_18);

    iText(125,80,"(6) Click the  UP ARROW KEY  to stop and start the music",GLUT_BITMAP_HELVETICA_18);

    iSetColor(97,97,97);
    iFilledRectangle(1000,350,220,150);
    iSetColor(255,255,255);
    iText(1050,415,"LET'S START",GLUT_BITMAP_HELVETICA_18);
}

void timer()
{
    tm -=1;
    sprintf(s_time,"%d",tm);
}

void draw_pause()
{
   iFilledRectangle(680,740,45,30);
   iSetColor(255,255,255);
   iText(682,752,"PAUSE",GLUT_BITMAP_HELVETICA_12);
}

void draw_resume()
{
    iSetColor(255,85,85);
    iFilledEllipse(690,95,140,48,100);
    iSetColor(255,255,255);
    iText(650,90,"RESUME",GLUT_BITMAP_HELVETICA_18);
}

void draw_exit()
{
    iSetColor(255,85,85);
    iFilledEllipse(690,280,140,40,100);
    iSetColor(255,255,255);
    iText(670,275,"EXIT",GLUT_BITMAP_HELVETICA_18);
}

void draw_restart()
{
    iSetColor(255,85,85);
    iFilledEllipse(690,195,140,40,100);
    iSetColor(255,255,255);
    iText(645,185,"RESTART",GLUT_BITMAP_HELVETICA_18);
}

void draw_score()
{
    iSetColor(255,85,85);
    iFilledEllipse(690,95,140,48,100);
    iSetColor(255,255,255);
    iText(610,90,"YOUR SCORE:",GLUT_BITMAP_HELVETICA_18);
    iText(745,90,pnt,GLUT_BITMAP_HELVETICA_18);
}

int blue_egg=0,golden_egg=0;

void draw_blue_egg()
{
    blue_egg=1;
}

void draw_golden_egg()
{
    golden_egg=1;
}

void draw_egg(int x,int y,int type)
{

    if(type==1)
    {
        iSetColor(255,215,0);
        iFilledEllipse(x,y,10,15,100);
    }

    else if(type==2)
    {
        iSetColor(0,0,225);
        iFilledEllipse(x,y,10,15,100);
    }

    else if(type==3)
    {
        iSetColor(139,69,19);
        iFilledEllipse(x+5,y+16,5,3);
        iFilledEllipse(x+3,y+10,7,5,20);
        iFilledEllipse(x,y,12,7,20);
    }

    else{

        iSetColor(255,160,100);
        iFilledEllipse(x,y,10,15,100);
    }
}

void egg1_droping(int hen_x)
{
    int i,j=array_size(hen1_poss)-1;

    if(hen1_direction==1){

    for(int k=0;k<len;k++)
    {
        present[k][1]=0;
        egg1_y[k][1]=645;
    }

    for(i=0;i<len;i++)
    {
        if(hen_x==hen1_poss[i])
        {
            present[i][0]=1;
            if(tm%12==0)
            {
                if(tm%12==0) type[i][0]=1;
                else type[i][0]=2;
            }

            else if(tm%27==0) type[i][0]=1;

            else if(tm%17==0)
            {
                break_egg[i][0]=0;
                type[i][0]=3;
            }
            //if(i==len-2) hen1_direction=-1;
        }

        if( egg1_poss_x[i]>basket_x && egg1_poss_x[i]<basket_x+65 &&  egg1_y[i][0]<basket_y+80)
        {
            if(type[i][0]==1) point +=10;
            else if(type[i][0]==2) point +=5;
            else if(type[i][0]==3) point -=10;
            else point++;

            present[i][0]=0;
            egg1_y[i][0]=645;
            type[i][0]=0;
            //iSetColor(255,255,255);
            //iText(1000,600,"Points:");
        }

        else if(egg1_y[i][0]<180)
        {
            if(type[i][0]!=3) break_egg[i][0]=1;
            present[i][0]=0;
            egg1_y[i][0]=645;
            type[i][0]=0;
        }

        if(present[i][0])
        {
            //if(blue_egg==1) type=1;

            draw_egg(egg1_poss_x[i]+60,egg1_y[i][0],type[i][0]);
            egg1_y[i][0] -=dy;
        }
    }
  }


   else if(hen1_direction==-1){

    for(int k=0;k<len;k++)
    {
        present[k][0]=0;
        egg1_y[k][0]=645;
    }

    for(j;j>0;j--)
    {
        if(hen_x==hen1_poss[j])
        {
            present[j][1]=1;
            if(tm%12==0){
                    if(tm%27==0) type[j][1]=1;
                    else type[j][1]=2;
            }

            else if(tm%27==0) type[j][1]=1;

            else if(tm%17==0) {
                    break_egg[j][1]=0;
                    type[j][1]=3;
            }
            //if(j==1) hen1_direction=1;
        }

        if( egg1_poss_x[j]>basket_x && egg1_poss_x[j]<basket_x+80 &&  egg1_y[j][1]<basket_y+80)
        {
            if(type[j][1]==1) point +=10;
            else if(type[j][1]==2) point +=5;
            else if(type[j][1]==3) point -=10;
            else point++;

            present[j][1]=0;
            egg1_y[j][1]=645;
            type[j][1]=0;
        }

        else if(egg1_y[j][1]<180)
        {
            if(type[j][1]!=3) break_egg[j][1]=1;
            present[j][1]=0;
            egg1_y[j][1]=645;
            type[j][1]=0;
        }

        if(present[j][1])
        {
            draw_egg(egg1_poss_x[j]+60,egg1_y[j][1],type[j][1]);
            egg1_y[j][1] -=dy;
        }

       /* if(egg1_y[j][1]<180)
        {
            present[j][1]=0;
            egg1_y[j][1]=645;
            type[j][1]=0;
        }*/
    }

   //hen1_direction==1;
  }

}

int image;

int x=0,draw_hen;
void hen1_moving()
{

    draw_hen=1;

    if(hen1_direction==1)
    {
            x++;

            hen1x=hen1_poss[x];

    }

    if(x>=array_size(hen1_poss))
    {
        hen1x=hen1_poss[array_size(hen1_poss)-1];
        hen1_direction = -1;
    }

    if(hen1_direction==-1)
    {
        x--;

        hen1x=hen1_poss[x];
    }

    if(x<0){
            hen1x=hen1_poss[0];
            hen1_direction=1;
    }

}

int egg_dy=10;


void point_cnt(char pnt[])
{
    //itoa(point,pnt,10);
    sprintf(pnt,"%d",point);
    //strcat(points,pnt);
}

char background[]="bg2.bmp";
char basket[]="nbasket.bmp";
char egg[]="egg.bmp";
char br_egg[]="break_eggn2.bmp";
char home_page[]="homepage.bmp";

int show_home_page=1,inst_page=1;


//iDraw fnc starts here
void iDraw()
{
    iClear();

    if(tm==-1)
    {
        show_home_page=1;
        //exit(0);
    }



    if(show_home_page==1){

            iShowBMP(45,0,home_page);
            if(resume && tm!=-1)
            {
                draw_exit();
                draw_restart();
                draw_resume();
            }

            else if(tm==-1)
            {
                draw_exit();
                draw_restart();
                draw_score();
            }
            iPauseTimer(0);
    }

    else{

    if(inst_page==0){
    iResumeTimer(0);

    iShowBMP(50,0,background);

    iSetColor(255,0,0);
    iText(55,740,"TIME:",GLUT_BITMAP_HELVETICA_18);
    iText(110,740,s_time,GLUT_BITMAP_HELVETICA_18);

    draw_pause();

    iSetColor(0,0,0);
    iFilledRectangle(50,645,1290,5);

    if(draw_hen) iShowBMP2(hen1x+5,hen1y-20,hen,0xffffff);

    egg1_droping(hen1x);
    point_cnt(pnt);

    iSetColor(255,0,0);
    iText(1180,740,"POINTS: ",GLUT_BITMAP_HELVETICA_18);
    iText(1265,740,pnt,GLUT_BITMAP_HELVETICA_18);


    for(int i=0;i<len;i++)
    {
        if(break_egg[i][0] && hen1_direction==1)
        {
            iShowBMP2(egg1_poss_x[i],168,br_egg,0xffffff);
            break_egg[i][0]=0;
        }

        else if(break_egg[i][1] && hen1_direction==-1)
        {
            iShowBMP2(egg1_poss_x[i],168,br_egg,0xffffff);
            break_egg[i][1]=0;
        }
    }

    iShowBMP2(basket_x,basket_y,basket,0xffffff);

   }

  else if(inst_page==1)
  {
      instruction_page();

      if(image==1){

            iSetColor(255,255,255);
            iRectangle(300,600,100,100);


      }

      if(image==2)
      {
           iSetColor(255,255,255);
           iRectangle(410,600,100,100);
           strncpy(hen,hen2,5);
      }

      if(image==3)
      {
          iSetColor(255,255,255);
          iRectangle(520,600,100,100);
          strncpy(hen,hen3,5);
      }

      if(tm_input==61)
      {
          tm=tm_input;
          iSetColor(255,255,255);
          iRectangle(400,535,40,40);
      }

      if(tm_input==121)
      {
          tm=tm_input;
          iSetColor(255,255,255);
          iRectangle(450,535,40,40);
      }

      if(tm_input==181)
      {
          tm=tm_input;
          iSetColor(255,255,255);
          iRectangle(500,535,40,40);
      }
  }

  }

}

//iDraw fnc ends here

void iMouseMove(int mx, int my)
{

}

/*
	function iMouse() is called when the user presses/releases the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
        printf("%d  %d\n",mx,my);
    }

    if(show_home_page){
    if(mx>535 && mx<845 && my>100 && my<200)
    {
        start=0;
        show_home_page=0;
    }
  }

    if(tm==-1)
    {
        if(mx>550 && mx<830 && my<280 && my>210){

            tm=tm_input;
            point=0;
            show_home_page=0;
        }

        if(mx>550 && mx<830 && my<350 && my>290)
        {
            exit(0);
        }
    }

    if(tm!=-1 && start!=1)
    {
        if(mx>550 && mx<830 && my<280 && my>210){

            tm=tm_input;
            point=0;
            show_home_page=0;
        }

        if(mx>550 && mx<830 && my<350 && my>290)
        {
            exit(0);
        }

        if(mx>680 && mx<725 && my>740 && my<770)
        {
            resume=1;
            show_home_page=1;
        }
    }

    //FROM INSTRUCTION PAGE

    if(inst_page){

        if(mx>400 && mx<440 && my>540 && my<580)
        {
            tm_input=61;
        }
        if(mx>450 && mx<490 && my>540 && my<580)
        {
            tm_input=121;
        }
        if(mx>500 && mx<540 && my>540 && my<580)
        {
            tm_input=181;
        }

        if(mx>300 && mx<400 && my>600 && my<700)
        {
            image=1;
            //iSetColor(255,255,255);
            //iRectangle(300,600,100,100);
        }

        if(mx>410 && mx<510 && my>600 && my<700)
        {
            image=2;
        }

        if(mx>520 && mx<620 && my>600 && my<700)
        {
            image=3;
        }

        if(mx>1070 && mx<1290 && my>350 && my<500)
        {
            inst_page=0;
        }
    }


	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		//place your codes here
		//printf("%d  %d\n",mx,my);
		//printf("%d %d\n",tracer_x[0],tracer_y[0]);
	}
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/

void iKeyboard(unsigned char key)
{
    switch(key) {

        case 'p':
        case 'P':
            iPauseTimer(0);
            break;
        case 'r':
        case 'R':
			iResumeTimer(0);
			break;
    }
}


void iSpecialKeyboard(unsigned char key)
{
    if(key==GLUT_KEY_F1)
    {
        show_home_page=!show_home_page;
        resume=1;
    }

	if(key == GLUT_KEY_END)
	{
		exit(0);
	}
	if(key == GLUT_KEY_LEFT)
	{
	    if(basket_x>50)
        basket_x-= basket_speed;
        else basket_x=50;
	}
	if(key == GLUT_KEY_RIGHT)
	{
	    if(basket_x<1200)
		basket_x+= basket_speed;
		else basket_x=1200;
	}

	if(key == GLUT_KEY_UP)
	{
	    if(musicOn){
            musicOn =0;
            PlaySound(0,0,0);
	    }
	    else {

             musicOn=1;
             PlaySound(TEXT("game_music.wav"),NULL,SND_LOOP | SND_ASYNC);
	    }
	}

	/*if(key == GLUT_KEY_DOWN)
	{
		pic_y--;
	}*/
	//place your codes for other keys here
}

int main()
{
    for(int i=0;i<len;i++)
    {
        for(int j=0;j<2;j++) egg1_y[i][j]=645;
    }

    //basket_x=X_MAX/2;
    //hen1x=hen1_poss[4];
    hen1y=660;


    //iSetTimer(5000,draw_blue_egg);
    //iSetTimer(11000,draw_golden_egg);

    iSetTimer(1000,timer);

    iSetTimer(600,hen1_moving);

    PlaySound(TEXT("game_music.wav"),NULL,SND_LOOP | SND_ASYNC);
    iInitialize(X_MAX,Y_MAX,"catch the egg");

    return 0;
}

