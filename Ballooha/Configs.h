/*
 *	Configs.h: 头文件
 *	指定一些配置参数
*/

#pragma once

// 球
#define MAX_BALL_NUM 50	// 球的最大数量
#define MAX_BALL_SIZE 50	// 球的最大尺寸（半径）
#define MAX_BALL_INIT_SIZE 10	// 球的最大初始尺寸
#define MIN_BALL_INIT_SIZE 3	// 球的最小初始尺寸
#define GROW_SPEED 1	// 成长速度（每次成长大小）
#define INIT_MOVE_ELAPSE 100	//	初始移动间隔时间（ms）
#define STEP_SIZE 50	// 移动步长

// 边界位置
#define LEFT_BORDER 100
#define RIGHT_BORDER 1200
#define UPPER_BORDER 100
#define LOWER_BORDER 600

// 颜色
#define COLOR_RED (RGB(255,0,0))
#define COLOR_BLUE (RGB(0,0,255))
#define COLOR_GREEN (RGB(0,255,0))
#define COLOR_WHITE (RGB(255, 255, 255))

// 其它参数
#define REFRESH_RATE 50	// 帧刷新速率（图像绘制时间间隔，单位ms）
#define REFRESH_TIMER_ID 0	// 刷新用定时器ID号