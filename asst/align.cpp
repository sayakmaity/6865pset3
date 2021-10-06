/* --------------------------------------------------------------------------
 * File:    align.cpp
 * Created: 2015-10-01
 * --------------------------------------------------------------------------
 *
 *
 *
 * ------------------------------------------------------------------------*/

#include "align.h"

using namespace std;

Image denoiseSeq(const vector<Image> &imSeq)
{
  // // --------- HANDOUT  PS03 ------------------------------
  // Basic denoising by computing the average of a sequence of images

  Image sizegetter(imSeq.at(0).width(), imSeq.at(0).height(), imSeq.at(0).channels());

  for (int x = 0; x < sizegetter.width(); x++)
  {
    for (int y = 0; y < sizegetter.height(); y++)
    {
      for (int z = 0; z < sizegetter.channels(); z++)
      {
        float valsum = 0.0;
        for (int idx = 0; idx < imSeq.size(); idx++)
        {
          valsum += imSeq.at(idx)(x, y, z);
        }
        sizegetter(x, y, z) = valsum / imSeq.size();
      }
    }
  }
  return sizegetter;
}

Image denoiseSeq2(const vector<Image> &imSeq)
{
  // // --------- HANDOUT  PS03 ------------------------------
  // Basic denoising by computing the average of a sequence of images

  Image sizegetter(imSeq.at(0).width(), imSeq.at(0).height(), imSeq.at(0).channels());

  for (int x = 0; x < sizegetter.width(); x++)
  {
    for (int y = 0; y < sizegetter.height(); y++)
    {
      for (int z = 0; z < sizegetter.channels(); z++)
      {
        float valsum = 0.0;
        for (int idx = 0; idx < imSeq.size(); idx++)
        {
          valsum += pow(imSeq.at(idx)(x, y, z), 2);
        }
        sizegetter(x, y, z) = valsum / imSeq.size();
      }
    }
  }
  return sizegetter;
}

Image logSNR(const vector<Image> &imSeq, float scale)
{
  // // --------- HANDOUT  PS03 ------------------------------
  // returns an image visualizing the per-pixel and
  // per-channel log of the signal-to-noise ratio scaled by scale.

  Image averageImage = denoiseSeq(imSeq);
  Image averageImage2 = denoiseSeq2(imSeq);

  Image sizegetter(imSeq.at(0).width(), imSeq.at(0).height(), imSeq.at(0).channels());

  for (int x = 0; x < sizegetter.width(); x++)
  {
    for (int y = 0; y < sizegetter.height(); y++)
    {
      for (int z = 0; z < sizegetter.channels(); z++)
      {
        float valsum = 0.0;

        for (int idx = 0; idx < imSeq.size(); idx++)
        {
          valsum += pow(imSeq.at(idx)(x, y, z) - averageImage(x, y, z), 2);
        }

        valsum /= (imSeq.size() - 1);
        sizegetter(x, y, z) = 10 * log10(averageImage2(x, y, z) / (valsum + 0.000001)) * scale;
      }
    }
  }
  return sizegetter;
}

vector<int> align(const Image &im1, const Image &im2, int maxOffset)
{
  // // --------- HANDOUT  PS03 ------------------------------
  // returns the (x,y) offset that best aligns im2 to match im1.

  int minX = 0;
  int minY = 0;
  float best = pow(10, 100000);
  for (int i = -maxOffset; i <= maxOffset; i++)
  {
    for (int j = -maxOffset; j <= maxOffset; j++)
    {
      float diff = 0;

      for (int x = maxOffset; x < im1.width() - maxOffset; x++)
      {
        for (int y = maxOffset; y < im1.height() - maxOffset; y++)
        {
          for (int z = 0; z < im1.channels(); z++)
          {
            float val1 = im1.smartAccessor(x + i, y + j, z, true);
            float val2 = im2.smartAccessor(x, y, z, true);

            diff += pow(val1 - val2, 2);
          }
        }
      }

      // cout << i << "," << j << " " << diff << endl;

      if (diff < best)
      {
        best = diff;
        minX = i;
        minY = j;
      }
    }
  }
  // cout << minX << "," << minY << " " << best << endl;
  vector<int> output;
  output.push_back(minX);
  output.push_back(minY);

  return output;
}


vector<int> fastAlign(const Image &im1, const Image &im2, int maxOffset)
{
  // // --------- HANDOUT  PS03 ------------------------------
  // returns the (x,y) offset that best aligns im2 to match im1.

  int minX = 0;
  int minY = 0;
  float best = pow(10, 100000);
  for (int i = -maxOffset; i <= maxOffset; i+=2)
  {
    for (int j = -maxOffset; j <= maxOffset; j+=2)
    {
      float diff = 0;

      for (int x = maxOffset; x < im1.width() - maxOffset; x+=2)
      {
        for (int y = maxOffset; y < im1.height() - maxOffset; y+=2)
        {
          for (int z = 0; z < im1.channels(); z++)
          {
            float val1 = im1.smartAccessor(x + i, y + j, z, true);
            float val2 = im2.smartAccessor(x, y, z, true);

            diff += pow(val1 - val2, 2);
          }
        }
      }

      // cout << i << "," << j << " " << diff << endl;

      if (diff < best)
      {
        best = diff;
        minX = i;
        minY = j;
      }
    }
  }
  // cout << minX << "," << minY << " " << best << endl;
  vector<int> output;
  output.push_back(minX);
  output.push_back(minY);

  return output;
}

Image alignAndDenoise(const vector<Image> &imSeq, int maxOffset)
{
  // // --------- HANDOUT  PS03 ------------------------------
  // Registers all images to the first one in a sequence and outputs
  // a denoised image even when the input sequence is not perfectly aligned.

  Image firstImage = imSeq.at(0);
  Image countImage = Image(firstImage.width(), firstImage.height());

  vector<vector<int>> alignments;

  vector<Image> aligned;
  aligned.push_back(firstImage);

  for (int i = 1; i < imSeq.size(); i++)
  {

    Image focus = imSeq.at(i);
    vector<int> offset = align(firstImage, focus, maxOffset);
    aligned.push_back(roll(focus, offset.at(0), offset.at(1)));
  }

  return denoiseSeq(aligned);
}

Image split(const Image &sergeyImg)
{
  // --------- HANDOUT  PS03 ------------------------------
  // 6.865 only:
  // split a Sergey images to turn it into one 3-channel image.

  int newWidth = sergeyImg.width();
  int newHeight = floor(sergeyImg.height() / 3);

  Image outlayer = Image(newWidth, newHeight, 3);

  for (int x = 0; x < newWidth; x++)
  {
    for (int y = 0; y < newHeight; y++)
    {
      outlayer(x, y, 2) = sergeyImg(x, y, 0);
      outlayer(x, y, 1) = sergeyImg(x, y + newHeight, 0);
      outlayer(x, y, 0) = sergeyImg(x, y + 2 * newHeight, 0);
    }
  }
  return outlayer;
}

Image sergeyRGB(const Image &sergeyImg, int maxOffset)
{
  // // --------- HANDOUT  PS03 ------------------------------
  // 6.865 only:
  // aligns the green and blue channels of your rgb channel of a sergey
  // image to the red channel. This should return an aligned RGB image

  int newWidth = sergeyImg.width();
  int newHeight = floor(sergeyImg.height() / 3);

  Image redlayer = Image(newWidth, newHeight, 1);
  Image bluelayer = Image(newWidth, newHeight, 1);
  Image greenlayer = Image(newWidth, newHeight, 1);

  for (int x = 0; x < newWidth; x++)
  {
    for (int y = 0; y < newHeight; y++)
    {
      bluelayer(x, y) = sergeyImg(x, y, 0);
      greenlayer(x, y) = sergeyImg(x, y + newHeight, 0);
      redlayer(x, y) = sergeyImg(x, y + 2 * newHeight, 0);
    }
  }

  Image BL_align = roll(bluelayer, align(redlayer, bluelayer, maxOffset).at(0), align(redlayer, bluelayer, maxOffset).at(1));
  Image GL_align = roll(greenlayer, align(redlayer, greenlayer, maxOffset).at(0), align(redlayer, greenlayer, maxOffset).at(1));

  Image output = Image(newWidth, newHeight, 3);
  for (int x = 0; x < redlayer.width(); x++)
  {
    for (int y = 0; y < redlayer.height(); y++)
    {
      output(x, y, 0) = redlayer(x, y);
      output(x, y, 1) = GL_align(x, y);
      output(x, y, 2) = BL_align(x, y);
    }
  }

  return output;
}

/**************************************************************
 //               DON'T EDIT BELOW THIS LINE                //
 *************************************************************/

// This circularly shifts an image by xRoll in the x direction and
// yRoll in the y direction. xRoll and yRoll can be negative or postive
Image roll(const Image &im, int xRoll, int yRoll)
{

  int xNew, yNew;
  Image imRoll(im.width(), im.height(), im.channels());

  // for each pixel in the original image find where its corresponding
  // location is in the rolled image
  for (int x = 0; x < im.width(); x++)
  {
    for (int y = 0; y < im.height(); y++)
    {

      // use modulo to figure out where the new location is in the
      // rolled image. Then take care of when this returns a negative
      // number
      xNew = (x + xRoll) % im.width();
      yNew = (y + yRoll) % im.height();
      xNew = (xNew < 0) * (imRoll.width() + xNew) + (xNew >= 0) * xNew;
      yNew = (yNew < 0) * (imRoll.height() + yNew) + (yNew >= 0) * yNew;

      // assign the rgb values for each pixel in the original image to
      // the location in the new image
      for (int z = 0; z < im.channels(); z++)
      {
        imRoll(xNew, yNew, z) = im(x, y, z);
      }
    }
  }

  // return the rolled image
  return imRoll;
}
