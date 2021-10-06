/* -----------------------------------------------------------------
 * File:    a3_main.cpp
 * Author:  Michael Gharbi <gharbi@mit.edu>
 * Created: 2015-09-30
 * -----------------------------------------------------------------
 *
 *
 *
 * ---------------------------------------------------------------*/

#include "Image.h"
#include "align.h"
#include "basicImageManipulation.h"
#include "demosaic.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// This is a way for you to test your functions.
// We will only grade the contents of demosaic.cpp and align.cpp
int main()
{
  // cout << "nothing done in a3_main.cpp, debug me !" << endl;
  // // Denoise ---------------------------
  // // Load sequence
  vector<Image> seq;
  int n_images = 16;
  for (int i = 1; i <= n_images; ++i)
  {
    ostringstream fname;
    // fname << "./Input/aligned-ISO400/1D2N-iso400-";
    // fname << "./Input/aligned-ISO3200/1D2N-iso3200-";
    fname << "./Input/green/noise-small-";
    fname << i;
    fname << ".png";
    seq.push_back(Image(fname.str()));
  }

  // Denoise
  // Image out = denoiseSeq(seq);
  // out.write("./Output/denoised-3200.png");

  // Image SNRIm = logSNR(seq, 1 / 30.0);
  // SNRIm.write("./Output/snr_map-400-f.png");

  Image denoisedAndAligned = alignAndDenoise(seq);
  denoisedAndAligned.write("./Output/green_slowAlignDenoise.png");
  // const Image mit1 = create_special();
  // const Image mit2 = create_special2();
  // // mit1.write("./Output/mit1.png");
  // // mit2.write("./Output/mit2.png");

  // vector<int> alignment = align(mit1, mit2, 25);

  // cout << alignment.at(0) << " " << alignment.at(1) << endl;

  // // Demosaic ---------------------------
  Image raw("./Input/raw/signs-small.png");
  Image green = basicGreen(raw, 1);
  green.write("./Output/demosaic_green.png");
  Image red = basicRorB(raw, 0, 0);
  red.write("./Output/demosaic_red.png");
  Image blue = basicRorB(raw, 1, 1);
  blue.write("./Output/demosaic_blue.png");
  Image rgb = basicDemosaic(raw, 1, 1, 1, 0, 0);
  rgb.write("./Output/demosaiced.png");

  Image green2 = edgeBasedGreen(raw, 1);
  green2.write("./Output/demosaic_green_edge.png");
  Image rgb2 = edgeBasedGreenDemosaic(raw, 1, 1, 1, 0, 0);
  rgb2.write("./Output/demosaiced_edge.png");
  Image red2 = greenBasedRorB(raw, green2, 0, 0);
  red2.write("./Output/demosaic_red_enhanced.png");

  Image rgb3 = improvedDemosaic(raw, 1, 1, 1, 0, 0);
  rgb3.write("./Output/demosaiced_greenbased.png");
  //
  // // Sergey ---------------------------
  Image sergeyImg("./Input/Sergey/00907v_third.png");
  Image rgb4 = split(sergeyImg);
  rgb4.write("./Output/Sergey_split.png");
  Image rgbAlign = sergeyRGB(sergeyImg,10);
  rgbAlign.write("./Output/Sergey_aligned.png");

  return 0;
}
