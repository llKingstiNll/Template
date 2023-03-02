// CGameObject.h
#pragma once

#include <opencv2/opencv.hpp>

class CGameObject {
protected:
   cv::Point2f _position;
   cv::Point2f _velocity;
   cv::Rect _shape;
   int _lives;
   int _radius;
public:
   void move();
   bool collide(CGameObject& obj);
   bool collide_wall(cv::Size board);
   void hit();
   int get_lives() { return _lives; }
   void set_lives(int lives) { _lives = lives; }
   void set_pos(cv::Point2f pos) { _position = pos; }
   cv::Point2f get_pos() { return _position; }
   void draw(cv::Mat &im);
};
