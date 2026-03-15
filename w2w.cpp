#include <iostream>  
#include <windows.h> //clear screen,sleep
#include <string>
using namespace std;

int getSafeInt(string message);

int availableFreshStock = 0;  // for NGO available
int availableSemiWasteStock = 0; // for industry

// ========================= COMMON NODE CLASSES =========================

// Food node for hotel linked list and quality stack
class FoodNode 
{
public:
    int id;
    string name;
    int quantity;
    int qualityScore; // 0-100
    FoodNode* next;

    FoodNode(int _id = 0, string _name = "", int _qty = 0, int _q = 0) 
	{
        id = _id;
        name = _name;
        quantity = _qty;
        qualityScore = _q;
        next = NULL;
    }
};

// Hotel node for linked list
class HotelNode 
{
public:
    int id;
    string name;
    string location;
    FoodNode* foodHead; // linked list of surplus food
    HotelNode* next;

    HotelNode(int _id = 0, string _name = "", string _loc = "") 
	{
        id = _id;
        name = _name;
        location = _loc;
        foodHead = NULL;
        next = NULL;
    }
};

// NGO node for queue
class NGONode 
{
public:
    int id;
    string name;
    int requiredQuantity;
    NGONode* next;

    NGONode(int _id = 0, string _name = "", int _req = 0) 
	{
        id = _id;
        name = _name;
        requiredQuantity = _req;
        next = NULL;
    }
};

// Delivery request node for queue (can be priority-sorted later)
class DeliveryNode 
{
public:
    int id;
    string type;      // "NGO" or "INDUSTRY"
    string receiver;  // NGO or industry name
    int quantity;
    int priority;     // smaller = higher priority
    DeliveryNode* next;

    DeliveryNode(int _id = 0, string _type = "", string _recv = "", int _qty = 0, int _prio = 0) 
	{
        id = _id;
        type = _type;
        receiver = _recv;
        quantity = _qty;
        priority = _prio;
        next = NULL;
    }
};

// Admin tree node
class AdminNode 
{
public:
    string role;      // "Head", "Hotel Manager", "NGO Manager", etc.
    string name;
    AdminNode* left;  // can represent one sub-role
    AdminNode* right; // another sub-role

    AdminNode(string _role = "", string _name = "") 
	{
        role = _role;
        name = _name;
        left = right = NULL;
    }
};

// ========================= STACK (QUALITY CHECK) =========================

class FoodStack 
{
private:
    FoodNode* top;

public:
    FoodStack() 
	{
        top = NULL;
    }

    bool isEmpty() 
	{
        return top == NULL;
    }

    void push(FoodNode* node) 
	{
        if (!node) return;
        node->next = top;
        top = node;
    }

    FoodNode* pop() 
	{
        if (isEmpty()) return NULL;
        FoodNode* temp = top;
        top = top->next;
        temp->next = NULL;
        return temp;
    }

    void display() 
	{
        cout << "\n--- Available Food Items ---\n"; //Food Stack (Top to Bottom)
        if (isEmpty()) 
		{
            cout << "No surplus food available\n"; //Stack is empty.
            return;
        }
        FoodNode* current = top;
        while (current) 
		{
            cout << "ID: " << current->id << endl;
            cout << "Name: " << current->name << endl;
            cout << "Quantity: " << current->quantity << endl;
            cout << "Quality: " << current->qualityScore << endl;
            current = current->next;
        }
    }
};

// ========================= QUEUE (NGO) =========================

class NGOQueue 
{
private:
    NGONode* front;
    NGONode* rear;

public:
    NGOQueue() 
	{
        front = rear = NULL;
    }

    bool isEmpty() 
	{
        return front == NULL;
    }

    void enqueue(int id, string name, int req) 
	{
        NGONode* node = new NGONode(id, name, req);
        if (isEmpty()) 
		{
            front = rear = node;
        } 
		else 
		{
            rear->next = node;
            rear = node;
        }
        cout << "NGO added successfully.\n";
    }

    NGONode* dequeue() 
	{
        if (isEmpty()) 
		{
            cout << "No active NGO food requests in system.\n"; //NGO queue is empty.
            return NULL;
        }
        NGONode* temp = front;
        front = front->next;
        if (!front) rear = NULL;
        temp->next = NULL;
        return temp;
    }

    void display() 
	{
        cout << "\n--- Pending NGO Food Requests ---\n"; // NGO Queue (Front to Rear) 
        if (isEmpty()) 
		{
            cout << "No active NGO food requests in system.\n"; //No NGOs in queue.
            return;
        }
        NGONode* current = front;
        while (current) 
		{
            cout << "ID: " << current->id << endl;
            cout << "Name: " << current->name<< endl;
            cout << "Required Quantity: " << current->requiredQuantity << "\n";
            current = current->next;
        }
    }
};

// ========================= QUEUE (DELIVERY) =========================

class DeliveryQueue 
{
private:
    DeliveryNode* front;
    DeliveryNode* rear;

public:
    DeliveryQueue() 
	{
        front = rear = NULL;
    }

    bool isEmpty() 
	{
        return front == NULL;
    }

    void enqueue(int id, string type, string recv, int qty, int priority) 
	{
        DeliveryNode* node = new DeliveryNode(id, type, recv, qty, priority);

        // Simple priority insertion (ascending priority value)
        if (isEmpty()) 
		{
            front = rear = node;
        } 
		else if (priority < front->priority) 
		{
            node->next = front;
            front = node;
        } 
		else 
		{
            DeliveryNode* current = front;
            DeliveryNode* prev = NULL;
            while (current && current->priority <= priority) 
			{
                prev = current;
                current = current->next;
            }
            if (!current) 
			{ // insert at end
                rear->next = node;
                rear = node;
            } 
			else 
			{
                prev->next = node;
                node->next = current;
            }
        }
        cout << "Delivery request added (priority " << priority << ").\n"; //enqueued
    }

    DeliveryNode* dequeue() 
	{
        if (isEmpty()) 
		{
            cout << "No deliveries scheduled.\n";
            return NULL;
        }
        DeliveryNode* temp = front;
        front = front->next;
        if (!front) rear = NULL;
        temp->next = NULL;
        return temp;
    }

    void display() 
	{
        cout << "\n--- Pending Deliveries (Priority Wise) ---\n"; //Delivery Queue (Priority Order)
        if (isEmpty()) 
		{
            cout << "No delivery requests.\n";
            return;
        }
        DeliveryNode* current = front;
        while (current) 
		{
            cout << "ID: " << current->id << endl;
            cout << "Type: " << current->type << endl;
            cout << "Receiver: " << current->receiver << endl;
            cout << "Qty: " << current->quantity << endl;
            cout << "Priority: " << current->priority << "\n";
            current = current->next;
        }
    }
};

// ========================= LINKED LIST (HOTELS & FOOD) =========================

class HotelList 
{
private:
    HotelNode* head;
    int hotelCounter;
    int foodCounter;

public:
    HotelList() 
	{
        head = NULL;
        hotelCounter = 1;
        foodCounter = 1;
    }

    HotelNode* findHotelById(int id) 
	{
        HotelNode* curr = head;
        while (curr) 
		{
            if (curr->id == id) return curr;
            curr = curr->next;
        }
        return NULL;
    }

    void registerHotel() 
	{
        string name, location;
        cout << "Enter hotel name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter hotel location: ";
        getline(cin, location);

        HotelNode* node = new HotelNode(hotelCounter++, name, location);
        if (!head) 
		{
            head = node;
        } 
		else 
		{
            HotelNode* curr = head;
            while (curr->next) curr = curr->next;
            curr->next = node;
        }
        cout << "Hotel registered with ID: " << node->id << "\n";
    }

    void addSurplusFood() 
	{
        int id;
        id = getSafeInt("Enter hotel ID: ");
        
        HotelNode* hotel = findHotelById(id);
        if (!hotel) 
		{
            cout << "Hotel not found.\n";
            return;
        }

        string fname;
        int qty, qscore;
        cout << "Enter food name: ";
        cin.ignore();
        getline(cin, fname);
        cout << "Enter quantity (in kg): ";
        cin >> qty;
        cout << "Enter quality score (0-100): ";
        cin >> qscore;

        FoodNode* food = new FoodNode(foodCounter++, fname, qty, qscore);
        // insert at beginning for simplicity
        food->next = hotel->foodHead;
        hotel->foodHead = food;

        cout << "Food item added successfully!\n";
		cout << "Food ID: " << food->id << "\n";
		cout << "Hotel: " << hotel->name << "\n";

    }

    void removeFoodItem() 
	{
        int hid, fid;
        hid = getSafeInt("Enter hotel ID: ");
        HotelNode* hotel = findHotelById(hid);
        if (!hotel) 
		{
			
            cout << "Hotel not found.\n";
            return;
        }
        cout << "\nAvailable food in this hotel:\n";

		FoodNode* temp = hotel->foodHead;
		if (!temp)
		{
		    cout << "No food available in this hotel.\n";
		    return;
		}

		while (temp)
		{
		    cout << "Food ID: " << temp->id << endl;
		    cout << " | Name: " << temp->name  << endl;
		    cout << " | Quantity: " << temp->quantity << " kg\n" << endl;
    		temp = temp->next;
		}

        fid = getSafeInt("Enter Food ID to remove: ");


        FoodNode* curr = hotel->foodHead;
        FoodNode* prev = NULL;
        while (curr && curr->id != fid) 
		{
            prev = curr;
            curr = curr->next;
        }
        if (!curr) 
		{
            cout << "Food item not found.\n";
            return;
        }
        if (!prev) 
		{
            hotel->foodHead = curr->next;
        } 
		else 
		{
            prev->next = curr->next;
        }
        delete curr;
        cout << "Food item removed.\n";
    }

    void displayHotels() 
	{
        cout << "\n--- Registered Hotels & Food ---\n";
        if (!head) 
		{
            cout << "No hotels registered.\n";
            return;
        }
        HotelNode* curr = head;
        while (curr) 
		{
            cout << "Hotel ID: " << curr->id << endl;
            cout << "Name: " << curr->name << endl;
            cout << "Location: " << curr->location << "\n";
            FoodNode* f = curr->foodHead;
            if (!f) 
			{
                cout << "  No surplus food.\n";
            } 
			else 
			{
                while (f) 
				{
                    cout << " Food ID: " << f->id << endl;
                    cout << " Name: " << f->name << endl;
                    cout << " Quantity: " << f->quantity << endl;
                    cout << " Quality: " << f->qualityScore << "\n";
                    f = f->next;
                }
            }
            curr = curr->next;
        }
    }

    // Move all food from hotels into a stack for quality check
    void moveFoodToStack(FoodStack& stack) 
	{
        HotelNode* h = head;
        while (h) 
		{
            FoodNode* curr = h->foodHead;
            while (curr) 
			{
                FoodNode* nextFood = curr->next;
                curr->next = NULL;
                stack.push(curr);
                curr = nextFood;
            }
            h->foodHead = NULL;
            h = h->next;
        }
        cout << "All surplus food transferred to quality inspection.\n"; //moved to quality check stack.
    }
};

// ========================= ARRAY (FERTILIZER INDUSTRIES) =========================

class FertilizerIndustryArray 
{
private:
    static const int MAX_IND = 10; // fixed size
    int count;

public:
    struct Industry 
	{
        int id;
        string name;
        string location;
        int purchasedQty;
    } industries[MAX_IND];

    FertilizerIndustryArray() 
	{
        count = 0;
    }

    void registerIndustry() 
	{
        if (count >= MAX_IND) 
		{
            cout << "Industry list is full.\n";
            return;
        }
        string name, loc;
        cout << "Enter industry name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter industry location: ";
        getline(cin, loc);

        industries[count].id = count + 1;
        industries[count].name = name;
        industries[count].location = loc;
        industries[count].purchasedQty = 0;
        cout << "Industry registered with ID: " << industries[count].id << "\n";
        count++;
    }

    Industry* findIndustryById(int id) 
	{
        for (int i = 0; i < count; i++) 
		{
            if (industries[i].id == id) return &industries[i];
        }
        return NULL;
    }

    void purchaseWasteFood(int industryId, int qty) 
	{
        Industry* ind = findIndustryById(industryId);
        if (!ind) 
		{
            cout << "Industry not found.\n";
            return;
        }
        ind->purchasedQty += qty;
        cout << "Industry " << ind->name << " purchased " << qty << " units.\n";
    }

    void displayIndustries() 
	{
        cout << "\n--- Fertilizer Industries ---\n";
        if (count == 0) 
		{
            cout << "No industries registered.\n";
            return;
        }
        for (int i = 0; i < count; i++) 
		{
            cout << "ID: " << industries[i].id << endl;
            cout << "Name: " << industries[i].name << endl;
            cout << "Location: " << industries[i].location << endl;
            cout << "Purchased Qty: " << industries[i].purchasedQty << "\n";
        }
    }
};

// ========================= ADMIN TREE =========================

class AdminTree 
{
private:
    AdminNode* root;

    void printNode(AdminNode* node, int level)
    {
        if (!node) return;

        for (int i = 0; i < level; i++)
            cout << "   ";

        cout << "[" << node->role << "] - " << node->name << "\n";
    }

    void printHierarchy(AdminNode* node, int level)
    {
        if (!node) return;

        // left child
        printHierarchy(node->left, level + 1);

        // current node
        printNode(node, level);

        // right child
        printHierarchy(node->right, level + 1);
    }

public:
    AdminTree() 
    {
        root = new AdminNode("Head Admin", "System Admin");
        root->left = new AdminNode("Hotel Manager", "Hotel Admin");
        root->right = new AdminNode("NGO Manager", "NGO Admin");
        root->left->left = new AdminNode("Quality Supervisor", "Quality Admin");
        root->right->right = new AdminNode("Industry Manager", "Industry Admin");
    }

    void showHierarchy() 
    {
        cout << "\n========== WASTE2WORTH ADMIN TEAM ==========\n";
        printHierarchy(root, 0);
        cout << "===========================================\n";
    }
};  

// ========================= SORTING UTILITIES =========================

// Simple selection sort for NGOs by required quantity (ascending)
void sortNgoByRequirement(NGONode* head) 
{
    for (NGONode* i = head; i != NULL; i = i->next) 
	{
        NGONode* minNode = i;
        for (NGONode* j = i->next; j != NULL; j = j->next) 
		{
            if (j->requiredQuantity < minNode->requiredQuantity)
                minNode = j;
        }
        if (minNode != i) 
		{
            // swap data only (not nodes)
            swap(i->id, minNode->id);
            swap(i->name, minNode->name);
            swap(i->requiredQuantity, minNode->requiredQuantity);
        }
    }
}

// ========================= QUALITY CHECK LOGIC =========================

class QualityCheckModule 
{
public:
    // Thresholds can be tuned
    // >=70 -> Fresh (Donation)
    // 40-69 -> Semi-waste (Industry)
    // <40 -> Waste (Discard)
    void processFood(FoodStack& stack, int& freshTotal, int& semiWasteTotal, int& wasteTotal, 
             int& availableFreshStock, int& availableSemiWasteStock) 
	{
    	freshTotal = semiWasteTotal = wasteTotal = 0;
    	availableFreshStock = availableSemiWasteStock = 0;  // Reset stocks
    
    	while (!stack.isEmpty()) 
    	{
    	    FoodNode* f = stack.pop();
    	    if (f->qualityScore >= 70) 
        	{
        	    freshTotal += f->quantity;
        	    availableFreshStock += f->quantity;
        	} 
        	else if (f->qualityScore >= 40) 
        	{
        	    semiWasteTotal += f->quantity;
        	    availableSemiWasteStock += f->quantity;
        	} 
        	else 
        	{
        	    wasteTotal += f->quantity;
        	}
        	delete f;
    	}
    
    	cout << "Quality check completed.\n";
    	cout << "Fresh for donation: " << freshTotal << " kg\n";
    	cout << "Semi-waste for industries: " << semiWasteTotal << " kg\n";
    	cout << "Waste to discard: " << wasteTotal << " kg\n";
    	cout << "Available Fresh Stock: " << availableFreshStock << " kg\n";
	} 	
};

// ========================= MAIN MENU SYSTEM =========================

void showBanner()
{
    cout << "\n";
    cout << "******************************* WASTE2WORTH ********************************\n";
    cout << "*  	                                                         	   *\n";
    cout << "* 	      Food Rescue & Redistribution System using DSA in C++         *\n";
    cout << "*  	                                                         	   *\n";
    cout << "*  	  Hotels -> Quality Check -> NGOs/Industries -> Delivery    	   *\n";
    cout << "*  	  Reduce Waste | Feed Hungry | Recycle for Fertilizer    	   *\n";
    cout << "*   	                                                        	   *\n";
    cout << "****************************************************************************\n\n";
}

int getSafeInt(string message)
{
    int value;
    while (true)
    {
        cout << message;
        cin >> value;

        if (!cin.fail())
        {
            return value;   // ✅ correct input
        }

        // ❌ invalid input
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input! Please enter a number only.\n";
    }
}

void hotelMenu(HotelList& hotels) 
{
    int choice = -1;
    while (choice != 0) 
	{
        cout << "\n=== Hotel Module ===\n"; // (Linked List)
        cout << "1. Register hotel\n";
        cout << "2. Add surplus food\n";
        cout << "3. Remove food item\n";
        cout << "4. View hotels and food\n";
        cout << "0. Back to main menu\n";
		do
		{
		    choice = getSafeInt("Enter choice: ");
		    if (choice < 0 || choice > 4)
    		    cout << "Please enter a valid option (0–4).\n";
		}
		while (choice < 0 || choice > 4);

        system("cls");
		showBanner();

        switch (choice) 
		{
        case 1: 
			hotels.registerHotel(); 
			Sleep(1500); 
			system("cls");
			showBanner();
			break;
        case 2: 
			hotels.addSurplusFood();
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();  
			system("cls");
			showBanner();
			break;
        case 3: 
			hotels.removeFoodItem(); 
			Sleep(1000); 
			system("cls");
			showBanner();
			break;
        case 4: 
			hotels.displayHotels(); 
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();
			system("cls");
			showBanner();
			break;
        case 0: 
			break;
        default: 
			cout << "Invalid choice.\n";
			Sleep(1000); 
			system("cls");
			showBanner();
        }
    }
}

void ngoMenu(NGOQueue& ngoQueue) 
{
    int choice = -1;
    while (choice != 0) 
	{
        cout << "\n=== NGO Module ===\n"; //(Queue) 
        cout << "1. Register NGO / Add request\n";
        cout << "2. Process next NGO request (dequeue)\n";
        cout << "3. View NGO queue\n";
        cout << "0. Back to main menu\n";
        do
		{
		    choice = getSafeInt("Enter choice: ");
		    if (choice < 0 || choice > 3)
    		    cout << "Please enter a valid option (0–3).\n";
		}
		while (choice < 0 || choice > 3);
		
        system("cls");
		showBanner();

        switch (choice) 
		{
        case 1: 
		{
            int id, req;
            string name;
            id = getSafeInt("Enter NGO ID: ");
            
            cout << "Enter NGO name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter required quantity (in kg): ";
            cin >> req;
            ngoQueue.enqueue(id, name, req);
            Sleep(1500); 
            system("cls");
			showBanner();
            break;
        }
		case 2: 
		{
    		NGONode* ngo = ngoQueue.dequeue();
    		if (ngo) 
    		{
        		cout << "Processing NGO: " << ngo->name 
            	 << " (Required: " << ngo->requiredQuantity << " kg)\n";
        
        		if (availableFreshStock >= ngo->requiredQuantity) 
        		{
            		availableFreshStock -= ngo->requiredQuantity;  // ? Deduct!
            		cout << " Delivered " << ngo->requiredQuantity << " kg\n";
            		cout << "Remaining Fresh Stock: " << availableFreshStock << " kg\n";
        		}
        		else 
        		{
            		cout << " Insufficient stock! Only " << availableFreshStock << " kg available.\n";
        		}
        		delete ngo;
    		}
    		Sleep(2000);
    		break;
        }
        case 3: 
			ngoQueue.display(); 
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get(); 
			system("cls");
			showBanner();
			break;
        case 0: 
			break;
        default: 
			cout << "Invalid choice.\n";
			Sleep(700); 
			system("cls");
			showBanner();
        }
    }
}

void industryMenu(FertilizerIndustryArray& industries) 
{
    int choice = -1;
    while (choice != 0) 
	{
        cout << "\n=== Fertilizer Industry Module ===\n"; // (Array)
        cout << "1. Register industry\n";
        cout << "2. Purchase waste food\n";
        cout << "3. View industries\n";
        cout << "0. Back to main menu\n";
        do
		{
		    choice = getSafeInt("Enter choice: ");
		    if (choice < 0 || choice > 3)
    		    cout << "Please enter a valid option (0–3).\n";
		}
		while (choice < 0 || choice > 3);
		
        system("cls");
		showBanner();

        switch (choice) 
		{
        case 1: 
			industries.registerIndustry(); 
			Sleep(1000); 
			system("cls");
			showBanner();
			break;
        case 2: 
		{
            int id, qty;
            
            id = getSafeInt("Enter industry ID: ");
            qty = getSafeInt("Enter quantity to purchase: ");
            
            industries.purchaseWasteFood(id, qty);
            Sleep(1500); 
            system("cls");
			showBanner();
            break;
        }
        case 3: 
			industries.displayIndustries(); 
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get(); 
			system("cls");
			showBanner();
			break;
        case 0: 
			break;
        default: 
			cout << "Invalid choice.\n";
			Sleep(700); 
			system("cls");
			showBanner();
        }
    }
}

void deliveryMenu(DeliveryQueue& deliveries) 
{
    int choice = -1;
    while (choice != 0) 
	{
        cout << "\n=== Delivery System ===\n"; // (Priority Queue) 
        cout << "1. Add delivery request\n";
        cout << "2. Process next delivery\n";
        cout << "3. View scheduled deliveries\n";
        cout << "0. Back to main menu\n";
        do
		{
		    choice = getSafeInt("Enter choice: ");
		    if (choice < 0 || choice > 3)
    		    cout << "Please enter a valid option (0–3).\n";
		}
		while (choice < 0 || choice > 3);
		
        system("cls");
		showBanner();

        switch (choice) 
		{
        case 1: 
		{
            int id, qty, prio;
            string type, recv;
            
            id = getSafeInt("Enter delivery ID: ");
            cout << "Enter type (NGO/INDUSTRY): ";
            cin >> type;
            cout << "Enter receiver name: ";
            cin.ignore();
            getline(cin, recv);
            cout << "Enter quantity: ";
            cin >> qty;
            cout << "Enter priority (1 = highest): ";
            cin >> prio;
            deliveries.enqueue(id, type, recv, qty, prio);
            Sleep(1000); 
            system("cls");
			showBanner();
            break;
        }
        case 2: 
		{
            DeliveryNode* d = deliveries.dequeue();
            if (d) 
			{
                cout << "Delivering " << d->quantity << " units to "
                     << d->receiver << " (" << d->type
                     << "), Priority: " << d->priority << "\n";
                delete d;
            }
            Sleep(1500); 
            system("cls");
			showBanner();
            break;
        }
        case 3: 
			deliveries.display(); 
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get(); 
			system("cls");
			showBanner();
			break;
        case 0: 
			break;
        default: 
			cout << "Invalid choice.\n";
			Sleep(700); 
			system("cls");
			showBanner();
        }
    }
}

void qualityMenu(HotelList& hotels, FoodStack& stack, QualityCheckModule& qcModule,
                 int& freshTotal, int& semiWasteTotal, int& wasteTotal,
                 int& availableFreshStock, int& availableSemiWasteStock)
				 {
    int choice = -1;
    while (choice != 0) 
	{
        cout << "\n=== Quality Check Module ===\n"; //(Stack)
        cout << "1. Send all hotel food for quality checking\n"; //Load all hotel food into stack
        cout << "2. View quality check items\n"; //View stack
        cout << "3. Run quality check\n";
        cout << "0. Back to main menu\n";
        do
		{
		    choice = getSafeInt("Enter choice: ");
		    if (choice < 0 || choice > 3)
    		    cout << "Please enter a valid option (0–3).\n";
		}
		while (choice < 0 || choice > 3);
		
        system("cls");
		showBanner();

        switch (choice) 
		{
        case 1: 
			hotels.moveFoodToStack(stack); 
			Sleep(1500); 
			system("cls");
			showBanner();
			break;
        case 2: 
			stack.display(); 
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get(); 
			system("cls");
			showBanner();
			break;
        case 3: 
			qcModule.processFood(stack, freshTotal, semiWasteTotal, wasteTotal, availableFreshStock, availableSemiWasteStock);
			cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get(); 
			system("cls");
			showBanner();
			break;
        case 0: 
			break;
        default: 
			cout << "Invalid choice.\n";
			Sleep(700); 
			system("cls");
			showBanner();
        }
    }
}

void adminMenu(AdminTree& adminTree,
               int freshTotal,
               int semiWasteTotal,
               int wasteTotal) 
{
    int choice = -1;
    while (choice != 0) 
    {
        cout << "\n=== Admin Module ===\n"; // (Tree & Reports)
        cout << "1. View Admin Hierarchy\n";
        cout << "2. View Quality Report (Detailed)\n";
        cout << "3. View System Performance\n";
        cout << "4. View Admin Suggestions / Alerts\n";
        cout << "0. Back to main menu\n";
        cout << "Enter choice: ";
        do
		{
		    choice = getSafeInt("Enter choice: ");
		    if (choice < 0 || choice > 4)
    		    cout << "Please enter a valid option (0–4).\n";
		}
		while (choice < 0 || choice > 4);
        showBanner();

        switch (choice) 
        {
        case 1: 
            adminTree.showHierarchy(); 
            cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();
            system("cls");
            showBanner();
            break;
        case 2:
        {
            int totalCollected = freshTotal + semiWasteTotal + wasteTotal;

            cout << "\n=========== QUALITY ANALYSIS REPORT ===========\n";
            if (totalCollected == 0) 
            {
                cout << "No food processed yet. Run Quality Check Module first.\n";
            }
            else 
            {
                double freshPercent     = (freshTotal     * 100.0) / totalCollected;
                double semiWastePercent = (semiWasteTotal * 100.0) / totalCollected;
                double wastePercent     = (wasteTotal     * 100.0) / totalCollected;

                cout << "Total Food Collected: " << totalCollected << " kg\n\n";

                cout << "Fresh for Donation : " << freshTotal     << " kg (" << freshPercent     << "%)\n";
                cout << "For Industries     : " << semiWasteTotal << " kg (" << semiWastePercent << "%)\n";
                cout << "Wasted / Discarded : " << wasteTotal     << " kg (" << wastePercent     << "%)\n";
                cout << "Total Processed: " << totalCollected << " kg\n";
				cout << "Available Fresh Stock: " << availableFreshStock << " kg\n";  // Real!
				cout << "Available Semi-Waste: " << availableSemiWasteStock << " kg\n";
            }

            cout << "-----------------------------------------------\n";
            cout << "Admin module acts as a monitoring and decision-\n";
            cout << "making unit. It analyzes food quality data,\n";
            cout << "detects wastage trends, and helps improve\n";
            cout << "overall system efficiency.\n";
            cout << "===============================================\n";

            cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();
            system("cls");
            showBanner();
            break;
        }
        case 3:
        {
            int totalCollected = freshTotal + semiWasteTotal + wasteTotal;

            cout << "\n=============== SYSTEM PERFORMANCE OVERVIEW ===============\n";

            if (totalCollected == 0) 
            {
                cout << "No performance data yet. Please run Quality Check\n";
                cout << "Module on available food.\n";
            }
            else 
            {
                double useful = freshTotal + semiWasteTotal; // donation + industry
                double usefulPercent = (useful * 100.0) / totalCollected;

                cout << "Total Food Processed : " << totalCollected << " kg\n";
                cout << "Useful (Donation + Industry): " << useful << " kg (" 
                     << usefulPercent << "%)\n";
                cout << "Pure Waste / Discarded      : " << wasteTotal << " kg\n\n";

                cout << "Admin can:\n";
                cout << "? Monitor food flow (hotel ? quality ? NGO/industry)\n";
                cout << "? Reduce waste by tracking quality scores\n";
                cout << "? Improve donation efficiency using reports\n";
                cout << "? Analyze NGO & industry usage patterns\n";
            }

            cout << "\n==========================================================\n";

            cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();
            system("cls");
            showBanner();
            break;
        }
        case 4:
        {
            int totalCollected = freshTotal + semiWasteTotal + wasteTotal;

            cout << "\n============= ADMIN SUGGESTIONS / ALERTS =============\n";

            if (totalCollected == 0) 
            {
                cout << "No data available. Please run the Quality Check\n";
                cout << "Module to generate reports.\n";
            }
            else 
            {
                if (wasteTotal > freshTotal) 
                {
                    cout << "? ALERT: Food wastage is high compared to fresh\n";
                    cout << "donation quality. Improve quality control at\n";
                    cout << "hotel level and review handling process.\n\n";
                }
                else 
                {
                    cout << "? System is working efficiently. Fresh donation\n";
                    cout << "quality is higher than wastage.\n\n";
                }

                if (semiWasteTotal > freshTotal) 
                {
                    cout << "Suggestion: A large portion is going to industries.\n";
                    cout << "Try to improve quality so more food can be donated\n";
                    cout << "to NGOs instead of being sold as semi-waste.\n\n";
                }

                if (wasteTotal > 0 && wasteTotal >= (0.2 * totalCollected)) 
                {
                    cout << "Suggestion: More than 20% of food is being wasted.\n";
                    cout << "Conduct a review of storage time, temperature, and\n";
                    cout << "transport to reduce spoilage.\n";
                }
            }

            cout << "======================================================\n";

            cout << "\nPress Enter to continue...";
			cin.ignore();
			cin.get();
            system("cls");
            showBanner();
            break;
        }
        case 0: 
            break;
        default: 
            cout << "Invalid choice.\n";
            Sleep(700); 
            system("cls");
            showBanner();
        }
    }
}

// ========================= MAIN =========================

int main() 
{
    HotelList hotels;
    NGOQueue ngoQueue;
    FertilizerIndustryArray industries;
    DeliveryQueue deliveries;
    FoodStack foodStack;
    QualityCheckModule qcModule;
    AdminTree adminTree;

    // Totals for admin report
    int freshTotal = 0;
    int semiWasteTotal = 0;
    int wasteTotal = 0;

    int choice = -1;
    while (choice != 0) 
	{
		system("cls");
		showBanner();
		cout << "1. Hotel Module \n"; //(Linked List)
        cout << "2. NGO Module \n"; //(Queue)
        cout << "3. Quality Check Module \n"; //(Stack)
        cout << "4. Fertilizer Industry Module \n"; //(Array)
        cout << "5. Delivery System \n"; //(Priority Queue)
        cout << "6. Admin Module \n"; //(Tree & Reports)
        cout << "0. Exit\n";
        do
		{
		    choice = getSafeInt("Enter choice: ");
		    if (choice < 0 || choice > 6)
    		    cout << "Please enter a valid option (0–6).\n";
		}
		while (choice < 0 || choice > 6);        
		
        switch (choice) 
		{
        case 1:
        	system("cls");
			showBanner();
			hotelMenu(hotels);
			break;
        case 2: 
        	system("cls");
			showBanner();
			ngoMenu(ngoQueue); 
			break;
        case 3: 
			system("cls");
			showBanner();
			qualityMenu(hotels, foodStack, qcModule, freshTotal, semiWasteTotal, wasteTotal, availableFreshStock, availableSemiWasteStock); 
			break;
        case 4:
			system("cls");
			showBanner(); 
			industryMenu(industries);
			break;
        case 5: 
			system("cls");
			showBanner();
			deliveryMenu(deliveries); 
			break;
        case 6: 
        	system("cls");
			showBanner();
			adminMenu(adminTree, freshTotal, semiWasteTotal, wasteTotal); 
			break;
        case 0: 
        	system("cls");
			showBanner();
			cout << "Exiting Waste2Worth. Goodbye!\n"; 
			Sleep(2000);
			break;
        default: 
        	system("cls");
			showBanner();
			cout << "Invalid choice.\n";
        }
    }

    return 0;
}
