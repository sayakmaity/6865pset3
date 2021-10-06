/* --------------------------------------------------------------------------
 * File:    demosaic.cpp
 * Created: 2015-10-01
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "demosaic.h"
#include <cmath>

using namespace std;

Image basicGreen(const Image &raw, int offset)
{
  // --------- HANDOUT  PS03 ------------------------------
  // Takes as input a raw image and returns a single-channel
  // 2D image corresponding to the green channel using simple interpolation

  Image output(raw.width(), raw.height(), 1);

  for (int x = 0; x < raw.width(); x++)
  {
    for (int y = 0; y < raw.height(); y++)
    {
      output(x, y) = raw(x, y, 0);
    }
  }

  for (int x = 1; x < raw.width() - 1; x++)
  {
    for (int y = 1; y < raw.height() - 1; y++)
    {

      if ((x + y + offset) % 2 == 0)
      {
        output(x, y) = raw(x, y);
      }
      else
      {
        output(x, y) = 0.25 * (raw(x + 1, y) + raw(x - 1, y) + raw(x, y + 1) + raw(x, y - 1));
      }
    }
  }

  return output;
}

Image basicRorB(const Image &raw, int offsetX, int offsetY)
{
  // --------- HANDOUT  PS03 ------------------------------
  //  Takes as input a raw image and returns a single-channel
  //  2D image corresponding to the red or blue channel using simple
  //  interpolation

  Image output(raw.width(), raw.height(), 1);

  for (int x = 0; x < raw.width(); x++)
  {
    for (int y = 0; y < raw.height(); y++)
    {
      output(x, y) = raw(x, y, 0);
    }
  }

  for (int x = 2; x < raw.width() - 2; x++)
  {
    for (int y = 2; y < raw.height() - 2; y++)
    {

      if ((x + offsetX) % 2 == 0 && (y + offsetY) % 2 == 0)
      {
        output(x, y) = raw(x, y);
      }
      else if ((x + offsetX) % 2 == 1 && (y + offsetY) % 2 == 1)
      {
        output(x, y) = 0.25 * (raw(x + 1, y + 1) + raw(x - 1, y + 1) + raw(x + 1, y - 1) + raw(x - 1, y - 1));
      }
      else if ((x + offsetX) % 2 == 0 && (y + offsetY) % 2 == 1)
      {
        output(x, y) = 0.5 * (raw(x, y + 1) + raw(x, y - 1));
      }
      else if ((x + offsetX) % 2 == 1 && (y + offsetY) % 2 == 0)
      {
        output(x, y) = 0.5 * (raw(x + 1, y) + raw(x - 1, y));
      }
    }
  }

  return output;
}

Image basicDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                    int offsetRedY, int offsetBlueX, int offsetBlueY)
{
  // --------- HANDOUT  PS03 ------------------------------
  // takes as input a raw image and returns an rgb image
  // using simple interpolation to demosaic each of the channels

  Image output(raw.width(), raw.height(), raw.channels());
  Image redchannel = basicRorB(raw, offsetRedX, offsetRedY);
  Image bluechannel = basicRorB(raw, offsetBlueX, offsetBlueY);
  Image greenchannel = basicGreen(raw, offsetGreen);

  for (int x = 0; x < raw.width(); x++)
  {
    for (int y = 0; y < raw.height(); y++)
    {
      output(x, y, 0) = redchannel(x, y);
      output(x, y, 1) = greenchannel(x, y);
      output(x, y, 2) = bluechannel(x, y);
    }
  }
  return output;
}

Image edgeBasedGreen(const Image &raw, int offset)
{
  // --------- HANDOUT  PS03 ------------------------------
  // Takes a raw image and outputs a single-channel
  // image corresponding to the green channel taking into account edges
  Image output(raw.width(), raw.height(), 1);

  for (int x = 0; x < raw.width(); x++)
  {
    for (int y = 0; y < raw.height(); y++)
    {
      output(x, y) = raw(x, y, 0);
    }
  }

  for (int x = 1; x < raw.width() - 1; x++)
  {
    for (int y = 1; y < raw.height() - 1; y++)
    {

      if ((x + y + offset) % 2 == 0)
      {
        output(x, y) = raw(x, y);
      }
      else if (abs(raw(x + 1, y) - raw(x - 1, y)) < abs(raw(x, y + 1) - raw(x, y - 1)))
      {
        output(x, y) = 0.5 * (raw(x + 1, y) + raw(x - 1, y));
      }
      else
      {
        output(x, y) = 0.5 * (raw(x, y + 1) + raw(x, y - 1));
      }
    }
  }

  return output;
}

Image edgeBasedGreenDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                             int offsetRedY, int offsetBlueX, int offsetBlueY)
{
  // --------- HANDOUT  PS03 ------------------------------
  // Takes as input a raw image and returns an rgb image
  // using edge-based green demosaicing for the green channel and
  // simple interpolation to demosaic the red and blue channels
  Image output(raw.width(), raw.height(), raw.channels());
  Image redchannel = basicRorB(raw, offsetRedX, offsetRedY);
  Image bluechannel = basicRorB(raw, offsetBlueX, offsetBlueY);
  Image greenchannel = edgeBasedGreen(raw, offsetGreen);

  for (int x = 0; x < raw.width(); x++)
  {
    for (int y = 0; y < raw.height(); y++)
    {
      output(x, y, 0) = redchannel(x, y);
      output(x, y, 1) = greenchannel(x, y);
      output(x, y, 2) = bluechannel(x, y);
    }
  }
  return output;
}

Image greenBasedRorB(const Image &raw, Image &green, int offsetX, int offsetY)
{
  // --------- HANDOUT  PS03 ------------------------------
  // Takes as input a raw image and returns a single-channel
  // 2D image corresponding to the red or blue channel using green based
  // interpolation

  Image output(raw.width(), raw.height(), 1);

  for (int x = 0; x < raw.width(); x++)
  {
    for (int y = 0; y < raw.height(); y++)
    {
      output(x, y) = raw(x, y, 0);
    }
  }

  for (int x = 2; x < raw.width() - 2; x++)
  {
    for (int y = 2; y < raw.height() - 2; y++)
    {

      if ((x + offsetX) % 2 == 0 && (y + offsetY) % 2 == 0)
      {
        output(x, y) = raw(x, y) - green(x,y);
      }
      else if ((x + offsetX) % 2 == 1 && (y + offsetY) % 2 == 1)
      {
        output(x, y) = 0.25 * (raw(x + 1, y + 1) + raw(x - 1, y + 1) + raw(x + 1, y - 1) + raw(x - 1, y - 1) - (green(x + 1, y + 1) + green(x - 1, y + 1) + green(x + 1, y - 1) + green(x - 1, y - 1)));
      }
      else if ((x + offsetX) % 2 == 0 && (y + offsetY) % 2 == 1)
      {
        output(x, y) = 0.5 * (raw(x, y + 1) + raw(x, y - 1) - (green(x, y + 1) + green(x, y - 1)));
      }
      else if ((x + offsetX) % 2 == 1 && (y + offsetY) % 2 == 0)
      {
        output(x, y) = 0.5 * (raw(x + 1, y) + raw(x - 1, y) - (green(x + 1, y) + green(x - 1, y)));
      }
    }
  }

  return output + green;
}

Image improvedDemosaic(const Image &raw, int offsetGreen, int offsetRedX,
                       int offsetRedY, int offsetBlueX, int offsetBlueY)
{
  // // --------- HANDOUT  PS03 ------------------------------
  // Takes as input a raw image and returns an rgb image
  // using edge-based green demosaicing for the green channel and
  // simple green based demosaicing of the red and blue channels
  Image output(raw.width(), raw.height(), raw.channels());
  Image greenchannel = edgeBasedGreen(raw, offsetGreen);

  Image redchannel = greenBasedRorB(raw, greenchannel, offsetRedX, offsetRedY);
  Image bluechannel = greenBasedRorB(raw, greenchannel, offsetBlueX, offsetBlueY);

  for (int x = 0; x < raw.width(); x++)
  {
    for (int y = 0; y < raw.height(); y++)
    {
      output(x, y, 0) = redchannel(x, y);
      output(x, y, 1) = greenchannel(x, y);
      output(x, y, 2) = bluechannel(x, y);
    }
  }
  return output;
}
