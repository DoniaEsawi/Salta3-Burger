#include "GUI.h"
//////////////////////////////////////////////////////////////////////////////////////////
GUI::GUI()
{
	int L = WindWidth / 2;
	DrawingItemsCount = 0;
	pWind = new window(WindWidth - 300, WindHeight - 120, 0, 0);
	pWind->ChangeTitle("The Restautant");
	//Set color for each order type
	DrawingColors[TYPE_NRM] = RED;	//normal-order color
	DrawingColors[TYPE_VGAN] = DARKBLUE;		//vegan-order color
	DrawingColors[TYPE_VIP] = VIOLETRED;		//VIP-order color					
	pWind->DrawImage("GUI\\bggg.jpg", 0, MenuBarHeight, WindWidth - 300, WindHeight - 120);
	int x, y;
	pWind->WaitMouseClick(x, y);
	delete pWind;
	pWind = new window(WindWidth +15, WindHeight, 0, 0);
	ClearStatusBar();
	ClearDrawingArea();
	DrawRestArea();

}
//////////////////////////////////////////////////////////////////////////////////////////
GUI::~GUI()
{
	delete pWind;
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== INPUT FUNCTIONS ====================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::waitForClick() const
{
	int x, y;
	pWind->WaitMouseClick(x, y);	//Wait for mouse click
}
//////////////////////////////////////////////////////////////////////////////////////////
string GUI::GetString() const
{
	string Label;
	char Key;
	while (1)
	{
		pWind->WaitKeyPress(Key);
		if (Key == 27)	//ESCAPE key is pressed
			return "";	//returns nothing as user has cancelled label
		if (Key == 13)	//ENTER key is pressed
			return Label;
		if ((Key == 8) && (Label.size() >= 1))	//BackSpace is pressed
			Label.resize(Label.size() - 1);
		else
			Label += Key;

		PrintMessage(Label,"Maiandra GD");
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
// ================================== OUTPUT FUNCTIONS ===================================
//////////////////////////////////////////////////////////////////////////////////////////

void GUI::PrintMessage(string msg,char* font) const	//Prints a message on status bar
{
	ClearStatusBar();	//First clear the status bar

	pWind->SetPen(Yellow);
	pWind->SetFont(32,PLAIN, BY_NAME,font);
	pWind->DrawString1(10, WindHeight - (int)(StatusBarHeight / 1.5), msg); // You may need to change these coordinates later 
																		  // to be able to write multi-line
}
void GUI::PrintMessage2(string msg, char* font) const	//Prints a message on status bar
{
	

	pWind->SetPen(Yellow);
	pWind->SetFont(32, PLAIN, BY_NAME, font);
	pWind->DrawString1(10, WindHeight - (int)(StatusBarHeight / 1.5), msg); // You may need to change these coordinates later 
																		  // to be able to write multi-line
}
void GUI::PrintMessage(string msg, int line) const
{
	pWind->SetPen(DARKBLUE);
	pWind->SetFont(20, BOLD, BY_NAME, "Times New Roman");

	if (line == 1)
	{
		
		//ClearStatusBar(1);
		pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.1), msg);
	}
	if (line == 2)
	{
		//ClearStatusBar(2);
		pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 1.3), msg);
	}
	if (line == 3)
	{
		//ClearStatusBar(3);
		pWind->DrawString(10, WindHeight - (int)(StatusBarHeight /1.6), msg);
	}
	if (line == 4)
	{
		//ClearStatusBar(4);
		pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 2.11), msg);
	}
	if (line == 5)
	{
		
		//ClearStatusBar(5);
		pWind->DrawString(10, WindHeight - (int)(StatusBarHeight / 2.95), msg);
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
void GUI::DrawString(const int iX, const int iY, const string Text)
{
	pWind->SetPen(BROWN);
	pWind->SetFont(22, BOLD, BY_NAME, "Arial");
	pWind->DrawString(iX, iY, Text);
}
void GUI::DrawString1(const int iX, const int iY, const string Text)
{
	pWind->SetPen(Yellow);
	pWind->SetFont(22, BOLD, BY_NAME, "Lucida Console");
	pWind->DrawString1(iX, iY, Text);
}
//////////////////////////////////////////////////////////////////////////////////////////
void GUI::ClearStatusBar() const
{
	/*pWind->SetPen(WHITE, 3);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(0, WindHeight - StatusBarHeight , WindWidth, WindHeight);	*/
	pWind->DrawImage("GUI\\d.jpg", 0, WindHeight - StatusBarHeight, WindWidth+2, StatusBarHeight);

	pWind->SetPen(DARKBLUE, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight, WindWidth, WindHeight - StatusBarHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::ClearDrawingArea() const
{
	// Clearing the Drawing area
	//pWind->SetPen(PINK, 3);
	//pWind->SetBrush(PINK);
	//pWind->DrawRectangle(0, MenuBarHeight, WindWidth, WindHeight - StatusBarHeight);
	pWind->DrawImage("GUI\\waiting3.jpg", 0, MenuBarHeight, (WindWidth / 2)-1, (WindHeight - StatusBarHeight) / 2-60);
	pWind->DrawImage("GUI\\waiting3-2.jpg", 0, MenuBarHeight + (WindHeight - StatusBarHeight) / 2-60, WindWidth / 2-60,59);
	pWind->DrawImage("GUI\\chef.jpg", WindWidth / 2+2, MenuBarHeight, WindWidth / 2, (WindHeight - StatusBarHeight) / 2-60);
	pWind->DrawImage("GUI\\chef-2.jpg", WindWidth / 2+61, MenuBarHeight + (WindHeight - StatusBarHeight) / 2 - 60, WindWidth / 2-58,59);
	pWind->DrawImage("GUI\\6.jpg", -1, MenuBarHeight + ((WindHeight - StatusBarHeight) / 2)+60, WindWidth / 2, (WindHeight - StatusBarHeight) / 2-61);
	pWind->DrawImage("GUI\\6-2.jpg", -1, MenuBarHeight + ((WindHeight - StatusBarHeight) / 2) + 1, WindWidth / 2-60, 60);
	pWind->DrawImage("GUI\\38.jpg", WindWidth / 2+2, MenuBarHeight + (WindHeight - StatusBarHeight) / 2 +60, WindWidth / 2+1, (WindHeight - StatusBarHeight) / 2-61);
	pWind->DrawImage("GUI\\38-2.jpg", WindWidth / 2+60, MenuBarHeight + (WindHeight - StatusBarHeight) / 2 + 1, WindWidth / 2 - 59, 60);
}
void GUI::ClearStatusBar(int line) const
{
	pWind->SetPen(WHITE, 3);
	pWind->SetBrush(WHITE);
	if (line == 1)
		pWind->DrawImage("GUI\\d.jpg",0, WindHeight - StatusBarHeight, WindWidth, WindHeight - (int)(StatusBarHeight / 1.3));
	if (line == 2)
		pWind->DrawImage("GUI\\d.jpg",0, WindHeight - (int)(StatusBarHeight / 1.3), WindWidth, WindHeight - (int)(StatusBarHeight / 1.5));
	if (line == 3)
		pWind->DrawImage("GUI\\d.jpg",0, WindHeight - (int)(StatusBarHeight / 1.56), WindWidth, WindHeight - (int)(StatusBarHeight / 1.9));
	if (line == 4)
		pWind->DrawImage("GUI\\d.jpg",0, WindHeight - (int)(StatusBarHeight / 1.9), WindWidth, WindHeight - (int)(StatusBarHeight / 2.7));
	if (line == 5)
		pWind->DrawImage("GUI\\d.jpg",0, WindHeight - (int)(StatusBarHeight / 3), WindWidth, WindHeight - (int)(StatusBarHeight / 4));
	
	pWind->SetPen(DARKBLUE, 3);
	pWind->DrawLine(0, WindHeight - StatusBarHeight, WindWidth, WindHeight - StatusBarHeight);
}
///////////////////////////////////////////////////////////////////////////////////
void GUI::DrawRestArea() const
{
	int L = RestWidth / 2;

	// 1- Drawing the brown square of the Rest
	pWind->SetPen(DARKBLUE);
	pWind->SetBrush(Yellow);
	pWind->DrawRectangle(RestStartX, RestStartY, RestEndX, RestEndY);

	// 2- Drawing the 2 brown crossed lines (for making 4 regions)
	pWind->SetPen(DARKBLUE, 3);
	pWind->DrawLine(0, YHalfDrawingArea, WindWidth, YHalfDrawingArea);
	pWind->DrawLine(WindWidth / 2, MenuBarHeight, WindWidth / 2, WindHeight - StatusBarHeight);

	// 3- Drawing the 2 white crossed lines (inside the Rest)
	pWind->SetPen(DARKBLUE);
	pWind->DrawLine(WindWidth / 2, YHalfDrawingArea - RestWidth / 2, WindWidth / 2, YHalfDrawingArea + RestWidth / 2);
	pWind->DrawLine(WindWidth / 2 - RestWidth / 2, YHalfDrawingArea, WindWidth / 2 + RestWidth / 2, YHalfDrawingArea);

	// 4- Drawing the 4 white squares inside the Rest (one for each region)
	/*pWind->SetPen(WHITE);
	pWind->SetBrush(WHITE);
	pWind->DrawRectangle(RestStartX , RestStartY , RestStartX + 2*L/2, RestStartY + 2*L/2);
	pWind->DrawRectangle(RestStartX + L/3, RestEndY - L/3, RestStartX + 2*L/3, RestEndY - 2*L/3);
	pWind->DrawRectangle(RestEndX - 2*L/3, RestStartY + L/3, RestEndX - L/3, RestStartY + 2*L/3);
	pWind->DrawRectangle(RestEndX - 2*L/3, RestEndY - L/3, RestEndX - L/3, RestEndY - 2*L/3);*/

	// 5- Writing regions labels
	pWind->SetPen(DARKBLUE);
	pWind->SetFont(22, PLAIN, BY_NAME, "jokerman");
	pWind->DrawString1(RestStartX + (int)(0.1 * L), RestStartY + 5 * L / 12, "WAIT");

	pWind->DrawString1(WindWidth / 2 + (int)(0.1 * L), RestStartY + 5 * L / 12, "COOK");

	pWind->DrawString1(WindWidth / 2 + (int)(0.1 * L), YHalfDrawingArea + 5 * L / 12, "SRVG");

	pWind->DrawString1(RestStartX + (int)(0.1 * L), YHalfDrawingArea + 5 * L / 12, "DONE");

}
//////////////////////////////////////////////////////////////////////////////////////////
//Draws the passed item in its region
//region count in the numbers of items drawn so far in that item's region
void GUI::DrawSingleItem(const DrawingItem* pDitem, int RegionCount) const       // It is a private function
{

	if (RegionCount > MaxRegionOrderCount)
		return; //no more items can be drawn in this region

	int DrawDistance = RegionCount;
	int YPos = 1;
	if (RegionCount >= MaxHorizOrders)	//max no. of orders to draw in one line
	{
		DrawDistance = (RegionCount - 1) % MaxHorizOrders + 1;
		YPos = (RegionCount - 1) / MaxHorizOrders + 1;
	}

	GUI_REGION Region = pDitem->region;

	int x, y, refX, refY;
	//First calculate x,y position of the order on the output screen
	//It depends on the region and the order distance
	switch (Region)
	{
	case ORD_REG:
		refX = (WindWidth / 2 - RestWidth / 2);
		refY = YHalfDrawingArea - OrderHeight; //
		x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
		y = refY - YPos * OrderHeight - YPos; // YPos
		break;
	case COOK_REG:
		refX = (WindWidth / 2 + RestWidth / 2);
		refY = YHalfDrawingArea - OrderHeight; //
		x = refX + (DrawDistance - 1) * OrderWidth + DrawDistance; //(Distance)
		y = refY - YPos * OrderHeight - YPos; // YPos
		break;
	case SRV_REG:
		refX = (WindWidth / 2 + RestWidth / 2);
		refY = YHalfDrawingArea + OrderHeight; //
		x = refX + (DrawDistance - 1) * OrderWidth + DrawDistance; //(Distance)
		y = refY + (YPos - 1) * OrderHeight + YPos; // YPos
		break;
	case DONE_REG:
		refX = (WindWidth / 2 - RestWidth / 2);
		refY = YHalfDrawingArea + OrderHeight; //
		x = refX - DrawDistance * OrderWidth - DrawDistance; //(Distance)
		y = refY + (YPos - 1) * OrderHeight + YPos; // YPos
		break;
	default:
		break;
	}

	// Drawing the item
	pWind->SetPen(pDitem->clr);
	pWind->SetFont(24, BOLD, BY_NAME,"Lucida Handwriting");
	pWind->DrawInteger(x, y, pDitem->ID);
}


//////////////////////////////////////////////////////////////////////////////////////////
/* A function to draw all items in DrawingList and ensure there is no overflow in the drawing*/
void GUI::DrawAllItems()
{

	//Prepare counter for each region
	int RegionsCounts[REG_CNT] = { 0 };	//initlaize all counters to zero

	DrawingItem* pDitem;
	for (int i = 0; i < DrawingItemsCount; i++)
	{
		pDitem = DrawingList[i];
		RegionsCounts[pDitem->region]++;
		DrawSingleItem(DrawingList[i], RegionsCounts[pDitem->region]);


	}

}

void GUI::UpdateInterface()
{
	ClearDrawingArea();
	DrawRestArea();
	DrawAllItems();
}

/*
	AddOrderForDrawing: Adds a new item related to the passed Order to the drawing list
*/
void GUI::AddToDrawingList(Order* pOrd)
{

	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pOrd->GetID();
	pDitem->clr = DrawingColors[pOrd->GetType()];
	ORD_STATUS order_status = pOrd->getStatus();
	GUI_REGION reg;

	switch (order_status)
	{
	case WAIT:
		reg = ORD_REG;	//region of waiting orders
		break;
	case SRV:
		reg = SRV_REG;	//region of waiting orders
		break;
	case DONE:
		reg = DONE_REG;	//region of waiting orders
		break;
	}

	pDitem->region = reg;

	DrawingList[DrawingItemsCount++] = pDitem;

}

void GUI::AddToDrawingList(Cook* pC)
{

	DrawingItem* pDitem = new DrawingItem;
	pDitem->ID = pC->GetID();
	pDitem->clr = DrawingColors[pC->GetType()];
	pDitem->region = COOK_REG;

	DrawingList[DrawingItemsCount++] = pDitem;

}

void GUI::ResetDrawingList()
{
	for (int i = 0; i < DrawingItemsCount; i++)
		delete DrawingList[i];

	DrawingItemsCount = 0;
}




PROG_MODE	GUI::getGUIMode() const
{
	PROG_MODE Mode;

	PrintMessage("Please select GUI mode: (1)Interactive, (2)StepByStep, (3)Silent");
	string S = GetString();

	
	Mode = (PROG_MODE)(atoi(S.c_str()) - 1);

	while (Mode < 0 || Mode >= 3)
	{
		pWind->DrawImage("GUI\\crop.jpg", 0, WindHeight - StatusBarHeight, WindWidth + 2, StatusBarHeight);

		pWind->SetPen(DARKBLUE, 3);
		pWind->DrawLine(0, WindHeight - StatusBarHeight, WindWidth, WindHeight - StatusBarHeight);
		PrintMessage2("Invalid input!,  Click to continue --->  " , "Cooper Black" );
		waitForClick();
		ClearStatusBar();
		Mode = getGUIMode();

	} 
		return Mode;
		
	
	

}
