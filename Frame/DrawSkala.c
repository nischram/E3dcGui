# include "/home/pi/E3dcGui/parameter.h"

int skala()
{
  int skalaLine2 = skalaLine * 2;
  int skalaLine3 = skalaLine * 3;
  int skalaLine4 = skalaLine * 4;
  int skalaLine5 = skalaLine * 5;
  int skalaLine6 = skalaLine * 6;
  double value = 330 / (double)PowerMax;
  drawSquare(15,430,769,1,LTGREY);
  drawSquareRGB(15, 430-(100*3.3), 769, 1, 0, 172, 0);
  drawSquareRGB(15, 430-(skalaLine*value), 769, 1, 200, 200, 200);
  if(skalaLine2 < PowerMax){
    drawSquareRGB(15, 430-(skalaLine2*value), 769, 1, 200, 200, 200);
  }
  if(skalaLine3 < PowerMax){
    drawSquareRGB(15, 430-(skalaLine3*value), 769, 1, 200, 200, 200);
  }
  if(skalaLine4 < PowerMax){
    drawSquareRGB(15, 430-(skalaLine4*value), 769, 1, 200, 200, 200);
  }
  if(skalaLine5 < PowerMax){
    drawSquareRGB(15, 430-(skalaLine5*value), 769, 1, 200, 200, 200);
  }
  if(skalaLine6 < PowerMax){
    drawSquareRGB(15, 430-(skalaLine6*value), 769, 1, 200, 200, 200);
  }
  drawSquare(15,430,1,4,LTGREY);
  drawSquare(23,430,1,2,LTGREY);
  drawSquare(31,430,1,2,LTGREY);
  drawSquare(39,430,1,2,LTGREY);
  drawSquare(47,430,1,4,LTGREY);
  drawSquare(55,430,1,2,LTGREY);
  drawSquare(63,430,1,2,LTGREY);
  drawSquare(71,430,1,2,LTGREY);
  drawSquare(79,430,1,4,LTGREY);
  drawSquare(87,430,1,2,LTGREY);
  drawSquare(95,430,1,2,LTGREY);
  drawSquare(103,430,1,2,LTGREY);
  drawSquare(111,430,1,4,LTGREY);
  drawSquare(119,430,1,2,LTGREY);
  drawSquare(127,430,1,2,LTGREY);
  drawSquare(135,430,1,2,LTGREY);
  drawSquare(143,430,1,4,LTGREY);
  drawSquare(151,430,1,2,LTGREY);
  drawSquare(159,430,1,2,LTGREY);
  drawSquare(167,430,1,2,LTGREY);
  drawSquare(175,430,1,4,LTGREY);
  drawSquare(183,430,1,2,LTGREY);
  drawSquare(191,430,1,2,LTGREY);
  drawSquare(199,430,1,2,LTGREY);
  drawSquare(207,430,1,4,LTGREY);
  drawSquare(215,430,1,2,LTGREY);
  drawSquare(223,430,1,2,LTGREY);
  drawSquare(231,430,1,2,LTGREY);
  drawSquare(239,430,1,4,LTGREY);
  drawSquare(247,430,1,2,LTGREY);
  drawSquare(255,430,1,2,LTGREY);
  drawSquare(263,430,1,2,LTGREY);
  drawSquare(271,430,1,4,LTGREY);
  drawSquare(279,430,1,2,LTGREY);
  drawSquare(287,430,1,2,LTGREY);
  drawSquare(295,430,1,2,LTGREY);
  drawSquare(303,430,1,4,LTGREY);
  drawSquare(311,430,1,2,LTGREY);
  drawSquare(319,430,1,2,LTGREY);
  drawSquare(327,430,1,2,LTGREY);
  drawSquare(335,430,1,4,LTGREY);
  drawSquare(343,430,1,2,LTGREY);
  drawSquare(351,430,1,2,LTGREY);
  drawSquare(359,430,1,2,LTGREY);
  drawSquare(367,430,1,4,LTGREY);
  drawSquare(375,430,1,2,LTGREY);
  drawSquare(383,430,1,2,LTGREY);
  drawSquare(391,430,1,2,LTGREY);
  drawSquare(399,430,1,4,LTGREY);
  drawSquare(407,430,1,2,LTGREY);
  drawSquare(415,430,1,2,LTGREY);
  drawSquare(423,430,1,2,LTGREY);
  drawSquare(431,430,1,4,LTGREY);
  drawSquare(439,430,1,2,LTGREY);
  drawSquare(447,430,1,2,LTGREY);
  drawSquare(455,430,1,2,LTGREY);
  drawSquare(463,430,1,4,LTGREY);
  drawSquare(471,430,1,2,LTGREY);
  drawSquare(479,430,1,2,LTGREY);
  drawSquare(487,430,1,2,LTGREY);
  drawSquare(495,430,1,4,LTGREY);
  drawSquare(503,430,1,2,LTGREY);
  drawSquare(511,430,1,2,LTGREY);
  drawSquare(519,430,1,2,LTGREY);
  drawSquare(527,430,1,4,LTGREY);
  drawSquare(535,430,1,2,LTGREY);
  drawSquare(543,430,1,2,LTGREY);
  drawSquare(551,430,1,2,LTGREY);
  drawSquare(559,430,1,4,LTGREY);
  drawSquare(567,430,1,2,LTGREY);
  drawSquare(575,430,1,2,LTGREY);
  drawSquare(583,430,1,2,LTGREY);
  drawSquare(591,430,1,4,LTGREY);
  drawSquare(599,430,1,2,LTGREY);
  drawSquare(607,430,1,2,LTGREY);
  drawSquare(615,430,1,2,LTGREY);
  drawSquare(623,430,1,4,LTGREY);
  drawSquare(631,430,1,2,LTGREY);
  drawSquare(639,430,1,2,LTGREY);
  drawSquare(647,430,1,2,LTGREY);
  drawSquare(655,430,1,4,LTGREY);
  drawSquare(663,430,1,2,LTGREY);
  drawSquare(671,430,1,2,LTGREY);
  drawSquare(679,430,1,2,LTGREY);
  drawSquare(687,430,1,4,LTGREY);
  drawSquare(695,430,1,2,LTGREY);
  drawSquare(703,430,1,2,LTGREY);
  drawSquare(711,430,1,2,LTGREY);
  drawSquare(719,430,1,4,LTGREY);
  drawSquare(727,430,1,2,LTGREY);
  drawSquare(735,430,1,2,LTGREY);
  drawSquare(743,430,1,2,LTGREY);
  drawSquare(751,430,1,4,LTGREY);
  drawSquare(759,430,1,2,LTGREY);
  drawSquare(767,430,1,2,LTGREY);
  drawSquare(775,430,1,2,LTGREY);
  drawSquare(783,430,1,4,LTGREY);
  return 1;
}
