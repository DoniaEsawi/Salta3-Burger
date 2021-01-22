#ifndef __GUI_H_
#define __GUI_H_

#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\Defs.h"

#include "..\Rest\Order.h"

#include "..\Rest\Cook.h"


#include "..\Generic_DS\Queue.h"

#include <string>
using namespace std;

class GUI
{
	enum GUI_REGION {
		ORD_REG,	//GUI Regions where waiting orders are drawn
		COOK_REG,		//GUI Regions where waiting coocks are drawn
		SRV_REG,	//GUI Regions where in-service orders are drawn
		DONE_REG,	//GUI Regions where finished orders are drawn
		REG_CNT		//Total number of regions
	};

	struct DrawingItem	//holds info for each item to be drawn on screen
	{
		int ID;		//ID to be printed on the screen indicating this item
		GUI_REGION region;	//Region where it should be drawn
		color	clr;	//drawing color
	};

private:
	window* pWind;


	color DrawingColors[TYPE_CNT];


	//The four regions in the GUI window: 


	// Some Static Constant Data Members: ---------------------

	static const int
		WindWidth = 1200, WindHeight =650,		//Window width and height
		StatusBarHeight = 150,	//Status Bar Height
		MenuBarHeight = 0,		//Menu Bar Height (distance from top of window to bottom line of menu bar)
		MenuItemWidth = 80,		//Width of each item in menu bar menu

		DrawingAreaHeight = WindHeight - MenuBarHeight - StatusBarHeight,
		YHalfDrawingArea = MenuBarHeight + DrawingAreaHeight / 2, //The y coordinate of half the Drawing area

		RestStartX = (int)(WindWidth * 0.45), //The x coordinate of the upper left corner of the Rest
		RestEndX = (int)(WindWidth * 0.55),   //The x coordinate of the lower right corner of the Rest

		RestWidth = RestEndX - RestStartX, //The width of the Rest (Note: the Rest is a Square)		

		RestStartY = YHalfDrawingArea - RestWidth / 2, //The y coordinate of the upper left corner of the Rest
		RestEndY = YHalfDrawingArea + RestWidth / 2,   //The y coordinate of the lower right corner of the Rest


		FontSize = 20,	//font size used to draw orders ID on Interface
		OrderWidth = 2 * FontSize,	//width of the order to be drawn on GUI
		OrderHeight = FontSize,		//height of the order to be drawn on GUI


		MaxHorizOrders = ((WindWidth - RestWidth) / 2) / (OrderWidth + 1),		//The max no. of orders the can be drwan in on Horizontal line in a region
		MaxVerticalOrders = (DrawingAreaHeight / 2) / (OrderHeight + 1),   //The max no. of orders the can be drwan in on Horizontal line in a region


		//Max no of orders that can be drawn in a single region
		MaxRegionOrderCount = MaxHorizOrders * MaxVerticalOrders;




	/////////
	////////
	////////


	static const int maxItemCnt = MaxPossibleOrdCnt + MaxPossibleMcCnt;
	DrawingItem* DrawingList[maxItemCnt];	 //List of items pointers to be drawn every timestep
	int DrawingItemsCount;	//actual no. of items in the drawing list

	//NOTES: 
	//Orders are assumed to be sorted by arrival time
	// At every time step, you should update those pointers 
	// to point to the current waiting orders only


	// 
	// TODO: Add more members if needed
	//

	void DrawSingleItem(const DrawingItem* pDitem, int RegionCount) const;		//draws ONE item 
	void DrawAllItems();		//drwas ALL items in DrawingList



	void DrawString(const int iX, const int iY, const string Text); // prints a message in the passed coordinates
	void DrawString1(const int iX, const int iY, const string Text);


	
	

public:
	GUI();
	~GUI();

	// Input Functions  ---------------------------
	void waitForClick() const;	// waits a user click from the user
	string GetString() const; // reads a string (keyboard input) from the user

	// Output Functions  ---------------------------
	void PrintMessage(string msg, char* font="Jokerman") const; // prints a message in the status bar
	void ClearStatusBar() const;    // clears the status bar
	void ClearDrawingArea() const;	// clears the Drawing area from all drawings
	void DrawRestArea() const;	    // draws the restaurant area
	void UpdateInterface();
	void AddToDrawingList(Order* pOrd);	//Adds a new order to the drawing queue
	void AddToDrawingList(Cook* pC);	//Adds a new cook to the drawing queue
	void ResetDrawingList();		//resets drawing list (should be called every timestep after drawing)
	void ClearStatusBar(int line) const;
	void PrintMessage(string msg, int line) const;
	PROG_MODE getGUIMode() const;			//returns the mode of the program
	void PrintMessage2(string msg, char* font = "Jokerman") const;
};

#endif