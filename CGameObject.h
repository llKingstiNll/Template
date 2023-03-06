// CGameObject.h
#pragma once

#include <opencv2/opencv.hpp>

/**
*
* @class CGameObject
*
* @brief A class representing a game object.
*
* This class represents a game object and contains attributes such as position, velocity, shape,
* lives, and radius. It can perform actions such as moving, changing velocity, detecting collisions,
* and drawing itself on an image.
* 
* @author Bailey Hung
*/
class CGameObject {
protected:
	cv::Point2f _position; ///< The position of the game object.
	cv::Point2f _velocity; ///< The velocity of the game object.
	cv::Rect _shape; ///< The shape of the game object.
	int _lives; ///< The number of lives of the game object.
	int _radius; ///< The radius of the game object. 

public:	
	/**
	* @brief Move the game object.
	* This method updates the position of the game object based on its velocity.
	* 
	* @return Nothing to return.
	*/
	void move();
	/**
	* @brief Change the velocity of the game object.
	* This method changes the velocity of the game object randomly.
	* 
	* @return Nothing to return.
	*/
	void changeVelocity();

	/**
	* @brief Check collision with another game object.
	* This method checks whether the game object collides with another game object.
	* 
	* @param obj The other game object to check collision with.
	* 
	* @return true if the game object collides with the other game object, false otherwise.
	*/
	bool collide(CGameObject& obj);

	/**
	* @brief Check collision with the wall.
	* This method checks whether the game object collides with the wall.
	* 
	* @param board The size of the board.
	* 
	* @return true if the game object collides with the wall, false otherwise.
	*/
	bool collide_wall(cv::Size board);

	/**
	* @brief Decrease the number of lives of the game object.
	* This method decreases the number of lives of the game object by 1.
	* 
	* @return Nothing to return
	*/
	void hit();

	/**
	* @brief Get the number of lives of the game object.
	* 
	* @return The number of lives of the game object.
	*/
	int get_lives() { return _lives; }

	/**
	* @brief Set the number of lives of the game object.
	* 
	* @param lives The new number of lives of the game object.
	* 
	* @retun Nothing to return.
	*/
	void set_lives(int lives) { _lives = lives; }

	/**
	* @brief Set the position of the game object.
	* 
	* @param pos The new position of the game object.
	* 
	* @return Nothing to return.
	*/
	void set_pos(cv::Point2f pos) { _position = pos; }

	/**
	* @brief Get the position of the game object.
	* 
	* @return The position of the game object.
	*/
	cv::Point2f get_pos() { return _position; }
	/**
    * @brief Draw the game object on an image.
	* 
	* @param im The image to draw the game object on.
	* 
	* @return Nothing to return.
	*/
	void draw(cv::Mat& im);
};

