// HashTable.cpp : Defines the entry point for the console application.
#include <iostream>
#include <vector>
#include<cstdlib>
#include<iomanip>
#include<iostream>
#include<cstdlib>
#include<fstream>
#include <sstream>
using namespace std;
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ 
template<class T>
class SetList                     // entire list
{
private:                        // private has the stucture of the element or node
    typedef struct element                // the individual node
    {
        T data;                 // defining the element
        struct element *next; // the pointer of the element
    } *nodePtr;
    //since structures and classes are literally creating a data type
    nodePtr globalHead, head;            // declare the head variable
public:
    SetList()                     // constructor, the first action
    {
        head = NULL;
    }         // instructs to start with an empty list
    /*~SetList()
    {
        clearSetList();
    }*/   
    //prototypes for link operation
    void appendElement(T);
    void removeElement(int);
    bool findElement(T);
    void clearSetList();
    void printSetList();
    T& at(int);
    int size() const;
    void pop_back();
    void insert(int pos, int i);
    void mergeSort() { mergeSort(&head); }
	void printList(); 
	int sum();
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    nodePtr mergeLists(nodePtr a, nodePtr b) {
        nodePtr mergedList = NULL;
        if (a == NULL) {
            return b;
        }
        else if (b == NULL) {
            return a;
        }
        if (a->data <= b->data) {
            mergedList = a;
            mergedList->next = mergeLists(a->next, b);
        }
        else {
            mergedList = b;
            mergedList->next = mergeLists(a, b->next);
        }
        return mergedList;
    }
    void mergeSort(nodePtr *source);
    void partition(nodePtr head, nodePtr *front, nodePtr *back);
//-------------------------------------------------------------------------------------------------------------
};

class BivariateTable
{
	private:
		SetList< SetList<float> > data;
		SetList<float> rowTotals;
		SetList<float> columnTotals;
		float total;
		
		// Calculates the row totals at the given index
		void calculateRowTotal(int index);
		// Calculates the column totals at the given index.
		void calculateColumnTotal(int index);
		// Calculates the grand total of the table
		void calculateTotal();
	public:
		// Constructor for the bivariate table. Sets up all the necessary items for it
		BivariateTable();
		// Destroys everything neatly so that there is no memory leaks.
		//~BivariateTable();
		
		// Increases the table's rows by 1
		void increaseRow();
		// Increases the table's columns by 1
		void increaseColumn();
		// Sets the number at the cell of the given coordinates (x, y). Updates the table
		void setCell(int x, int y, float num);
		// Gets the number at the cell of the given coordinates (x, y)
		float getCell(int x, int y);
		// Gets the width of the table (x)
		int width();
		// Gets the height of the table (y)
		int height();
		// Gets the row total at the given index
		float getRowTotal(int rowIndex);
		// Gets the column total at the given index
		float getColumnTotal(int colIndex);
		// Gets the grand total of the table
		float getTotal();
		// Destructively turn the table into a probability table
		void createProbability();
		
		// Displays the table
		void display();
		// Clones the table
		BivariateTable clone();
};

// Constructor for the bivariate table. Sets up all the necessary items for it
BivariateTable::BivariateTable()
{
	// This is where the main data will go. Everything from the cells of the csv will go in here
	data=	SetList< SetList<float> >();
	// Here is a seperate variable to hold all the totals of the rows. Access is only for the table
	rowTotals=	SetList<float>();
	// Here is a seperate variable to hold all the totals of the columns. Access is only for the table
	columnTotals=	SetList<float>();
	// Here is the grand total of the table. Access is only for the table
	total=	0;
}

// Destroys everything neatly so that there is no memory leaks.
// TODO: Make sure this works in the future. Just in case.
/*~BivariateTable::BivariateTable()
{
	delete data;
	delete rowTotals;
	delete columnTotals;
}*/

// Clones the table
BivariateTable BivariateTable::clone()
{
	// Variables
	BivariateTable	table=	BivariateTable();
	
	for(int a= 0; a< height(); a++)
	{
		table.increaseRow();
		for(int b= 0; b< width(); b++)
		{
			if(a== 0)
				table.data.at(a).appendElement(data.at(a).at(b));
			else
				table.data.at(a).at(b)=	data.at(a).at(b);
		}
	}
	for(int i= 0; i< rowTotals.size(); i++)
		table.calculateRowTotal(i);
	for(int i= 0; i< columnTotals.size(); i++)
		table.columnTotals.appendElement(columnTotals.at(i));
	table.total=	total;
	
	return table;
}

// Increases the table's rows by 1
void BivariateTable::increaseRow()
{
	SetList<float> elements= SetList<float>();
	
	for(int i= 0; i< width(); i++)
	{
		elements.appendElement(0); // This is to set all the column items to 0 to add a proper row
	}
	// Adds the row into the table
	data.appendElement(elements);
	// Adds a new row into the totals
	rowTotals.appendElement(0);
}

// Increases the table's columns by 1
void BivariateTable::increaseColumn()
{
	if(height()== 0)
		return;
	for(int i= 0; i< height(); i++)
	{
		data.at(i).appendElement(0); // This adds a blank 0 column into each row
	}
	// Adds a new column into the totals
	columnTotals.appendElement(0);
}

// Calculates the column totals at the given index.
void BivariateTable::calculateColumnTotal(int index)	{
	float totalTemp = 0;
	for( int i =0 ;i< height(); i++)
	{
		totalTemp+= data.at(i).at(index); // The sum of all the rows in a column at the given index
	}
	columnTotals.at(index)= totalTemp; // Sets the column total to what we just summed
}

// Calculates the row totals at the given index
void BivariateTable::calculateRowTotal(int index)	{
	float totalTemp = 0;
	for( int i =0 ;i< width(); i++)
	{
		totalTemp+= data.at(index).at(i); // The sum of all the columns in a row at the given index	
	}
	rowTotals.at(index)= totalTemp; // Sets the row total to what we just summed
}

// Calculates the grand total of the table
void BivariateTable::calculateTotal()
{
	float totalTemp= 0;
	for(int i= 0; i< rowTotals.size(); i++)
	{
		totalTemp+= rowTotals.at(i); // Sums all the numbers in the row total.
	}
	
	total=	totalTemp;
}

// Gets the width of the table (x)
int BivariateTable::width()
{
	// Precaution: if the table is of size 0, then exit. We do not want a crash here
	if(data.size()== 0)
		return 0;
	return data.at(0).size();
}

// Gets the height of the table (y)
int BivariateTable::height()
{
	return data.size();
}

// Gets the row total at the given index
float BivariateTable::getRowTotal(int rowIndex)
{
	// Checking if this is within bounds. Crashes if it ain't
	if(rowIndex< 0 || rowIndex>= rowTotals.size())
		throw "Index out of bounds";
	return rowTotals.at(rowIndex);
}

// Gets the column total at the given index
float BivariateTable::getColumnTotal(int colIndex)
{
	// Checking if this is within bounds. Crashes if it ain't
	if(colIndex< 0 || colIndex>= columnTotals.size())
		throw "Index out of bounds";
	return columnTotals.at(colIndex);
}

// Gets the grand total of the table
float BivariateTable::getTotal()
{
	return total;
}

// Gets the number at the cell of the given coordinates (x, y)
float BivariateTable::getCell(int x, int y)
{
	// Checking if this is within bounds. Crashes if it ain't
	if(x< 0 || x>= width())
		throw "Index out of bounds";
	if(y< 0 || y>= height())
		throw "Index out of bounds";
	
	return data.at(y).at(x);
}

// Sets the number at the cell of the given coordinates (x, y). Updates the table
void BivariateTable::setCell(int x, int y, float num)
{
	// Checking if this is within bounds. Crashes if it ain't
	if(x< 0 || x>= width())
		throw "Index out of bounds";
	if(y< 0 || y>= height())
		throw "Index out of bounds";
	
	data.at(y).at(x)= num;
	// Updates the row total at y
	calculateRowTotal(y);
	// Updates the column total at x
	calculateColumnTotal(x);
	// Updates the grand total
	calculateTotal();
}

// Destructively turn the table into a probability table
void BivariateTable::createProbability()
{
	// Goes through all the rows
	for(int a= 0; a< height(); a++)
	{
		// Goes through all the columns
		for(int b= 0; b< width(); b++)
		{
			// Sets the cell to be i/total, where i = cell(x, y).
			// In this case a = y and b = x
			data.at(a).at(b)= data.at(a).at(b)/total;
		}
	}
	// Does a recalculation because we set the data and the totals are completely wrong now.
	// Recalculates the row total
	for(int i= 0; i< rowTotals.size(); i++)
		calculateRowTotal(i);
	// Recalculates the column total
	for(int i= 0; i< columnTotals.size(); i++)
		calculateColumnTotal(i);
	// Recalculates the grand total
	calculateTotal();
}

// Displays the table
void BivariateTable::display()
{
	// Displays the x indices
    cout<< "    ";
	for(int i= 0; i< width(); i++)
	{
		cout<< setw(6)<< i;
	}
	cout<< endl;
	
	// Displays the data
    for(int a = 0; a < height(); a++)
    {
    	// This shows the y indices with a pipe next to it
       cout<< a<<" | ";
       // Displays the actual data
       for(int b= 0; b< width(); b++)
    	{
    		cout<< setw(6)<< data.at(a).at(b);
    	}
    	// Shows the row total
    	cout<< setw(6)<< rowTotals.at(a);
    	cout<< endl;
    }
    cout<< "    ";
    // Shows the column total
	for(int i= 0 ; i< width(); i++)
    {
    	cout<< setw(6)<< columnTotals.at(i);
    }
    // Shows the grand total
    cout<< setw(6)<< total;
    cout<< endl;
}


void readFromCSV(BivariateTable&, const char*);
void writeToCSV(BivariateTable&, const char*);
void writeToCSVExtra(BivariateTable&, const char*);
vector<float> split(const string);
BivariateTable theEye(BivariateTable&);

void readFromCSV(BivariateTable &table, const char* filename){
fstream dataFile; // file object
 string buffer; // Used to read line from file
 int y= 0;
/*
 // Create a new file named myfile.dat to write to.
 dataFile.open("myfile.dat", ios::out);

 // Write two lines to the file.
 dataFile << "Now is the time for all good men" << endl
 << "to come to the aid of their country.";
 // Close the file.
 dataFile.close();
*/
 // Open the file for input.
 dataFile.open(filename, ios::in);
if(!dataFile)
{
	cout<<"Not found"<<endl;
	cin.get();
}
while(!dataFile.eof()){
 // Read a line into a buffer and print the line.
 getline(dataFile, buffer);
 if(buffer== "")
 	continue;
 vector<float>	vect= split(buffer);
 table.increaseRow();
 while(table.width()< vect.size())	{
 	table.increaseColumn();
 }
 for(int i= 0; i< vect.size(); i++)
 	table.setCell(i, y, vect.at(i));
 
 
 y++;
}

 // Read a second line and print it.
 getline(dataFile, buffer);
 cout << buffer << endl;

 // Close the file.
 dataFile.close();
 }
 
 // Writes the table into a csv with other fancy things added in like probability table and such
 void writeToCSVExtra(BivariateTable& table, const char* filename)
 {
 	ofstream	file; // The file to write to
 	BivariateTable	probTable=	table.clone(); // A seperate cloned table used for the probability table
 	int	max=	0; // Used to get the max between the row and the column
 	double	Ex=	0, Ey=	0, Exy= 0; // Ex is the expected value of x. [ E(x) ]. While Ey is the same for y. [ E(y) ]. Exy is for [ E(xy) ].
 	
 	// Makes the cloned table into a probability table
 	probTable.createProbability();
 	// Opens up the file to the given filename
 	file.open(filename);
 	
 	file<< "\n\n"; // Added some spacing for you to see and write stuff
 	// Start of indexing the columns of the tables
	file<< ",";
 	for(int i= 0; i< table.width(); i++)
 		file<< i<< ","; // Writes the index of the columns of the original table
 	file<< ",,,"; // Spaces to properly get to the probability table
 	for(int i= 0; i< table.width(); i++)
 		file<< i<< ","; // Writes the index of the columns of the probability table
 	file<< "\n";
 	// End of indexing the columns of the tables
 	// Start of writing the data of the tables
 	for(int y= 0; y< table.height(); y++)
 	{
 		file<< y<< ","; // Starts out by writing the index of the rows of the tables
 		for(int x= 0; x< table.width(); x++)
 		{
 			file<< table.getCell(x, y); // Writes whats in the cell of the table
 			// Once we find that we are in the very last index of the table
 			// we add the row total, some space, and then proceed with the
 			// probability table
 			if(x== table.width()-1)
			 {
			 	// Writes down the row total, some space, and then the index of the rows of the probability table
 				file<< ","<< table.getRowTotal(y)<< ",,"<< y<< ",";
 				// Goes through the probability table
 				for(int z= 0; z< probTable.width(); z++)
 				{
 					file<< probTable.getCell(z, y); // Writes down the data from the probability table
 					// Once we find that we are at the very last index of the probability table
 					// we write the row totals
 					if(z== probTable.width()-1)
 					{
 						file<< ","<< probTable.getRowTotal(y);
 					}
 					// If we are still not at the very last index, then add a comma to seperate the cells
 					else
 						file<< ",";
 				}
 				// Make it go down a row
 				file<< "\n";
 			}
 			// If we are still not at the very last index, then add a comma to seperate the cells
 			else
 				file<< ",";
 		}
 	}
 	file<< ","; // Comma for spacing
 	// Writes down all the column totals of the original table
 	for(int i= 0; i< table.width(); i++)
 	{
 		file<< table.getColumnTotal(i)<< ",";
 	}
 	file<< table.getTotal()<< ",,,"; // Adds the grand total and some spacing
 	// Writes down all the column totals of the probability table
 	for(int i= 0; i< probTable.width(); i++)
 	{
 		file<< probTable.getColumnTotal(i)<< ",";
 	}
 	file<< probTable.getTotal()<< "\n\n"; // Adds the grand total and some spacing
 	
 	file<< "Unconditional Marginal Distribution\n";
 	file<< "Value of X,f(x),Value of Y,f(y)\n";
 	max=	probTable.width(); // We are getting the max for in case you use a non square table. nXm, where n != m
 	if(max< probTable.height()) // Checks for the max to get the max
 		max=	probTable.height();
 	for(int i= 0; i< max; i++)
 	{
 		file<< i<< ","; // Writes down the index
 		if(i< probTable.width()) // If i is within bounds, then get the column total of the probability table
 		{
 			file<< probTable.getColumnTotal(i);
 			Ex+=	probTable.getColumnTotal(i)*i;
 		}
		else // If i is out of bounds, then write in a zero
 			file<< "0";
 		file<< ","<< i<< ","; // Spacing and indexing
 		if(i< probTable.height()) // If i is within bounds, then get the column total of the probability table
 		{
		 	file<< probTable.getRowTotal(i);
		 	Ey+=	probTable.getRowTotal(i)*i;
 		}
 		else // If i is out of bounds, then write in a zero
 			file<< "0";
 		file<< "\n"; // Making a new line
 	}
 	file<< "E(x)=,"<< Ex<< ",E(y)=,"<< Ey<< "\n";
 	file<< "Conditional Probability\n";
 	for(int y= 1; y< probTable.height(); y++)
	 {
 		for(int x= 1; x< probTable.width(); x++)
 		{
 			Exy+=	x*y*probTable.getCell(x, y);
 		}
 	}
 	file<< "E(xy)=,"<< Exy;
 	
 	// Closes the file, if not it locks the file.
 	file.close();
 }
 
 
 // Writes the table into a csv with other fancy things added in like probability table and such
 void writeToCSV(BivariateTable& table, const char* filename)
 {
 	ofstream	file; // The file to write to
 	
	// Opens up the file to the given filename
 	file.open(filename);
 	
 	file<< "\n\n"; // Added some spacing for you to see and write stuff
 	// Start of indexing the columns of the tables
	file<< ",";
 	for(int i= 0; i< table.width(); i++)
 		file<< i/2<< ","; // Writes the index of the columns of the original table
 	file<< "\n";
 	// End of indexing the columns of the tables
 	// Start of writing the data of the tables
 	for(int y= 0; y< table.height(); y++)
 	{
 		file<< y<< ","; // Starts out by writing the index of the rows of the tables
 		for(int x= 0; x< table.width(); x++)
 		{
 			file<< table.getCell(x, y); // Writes whats in the cell of the table
 			if(x== table.width()-1)
			 {
 				file<< "\n";
 			}
 			// If we are still not at the very last index, then add a comma to seperate the cells
 			else
 				file<< ",";
 		}
 	}
 	
 	file.close();
 }
 
 vector<float> split(const string str)
 {
    std::vector<float> vect;
    std::stringstream ss(str);

    float i;

    while (ss >> i)
    {
        vect.push_back(i);

        if (ss.peek() == ',')
            ss.ignore();
    }
    return vect;
 }
 
 int mapPercentToIndex(float);

BivariateTable theEye(BivariateTable& observation)
{
	BivariateTable	observed=	BivariateTable();
	float	max=	0;
	int	maxIndex=	0;
	
	// Find the max
	for(int i= 0; i< observation.width(); i++)
	{
		if(max< observation.getCell(i, 0))
			max=	observation.getCell(i, 0);
		if(max< observation.getCell(i, 1))
			max=	observation.getCell(i, 1);
	}
	// Create table around max
	maxIndex=	mapPercentToIndex(max);
	for(int i= 0; i<= maxIndex; i++)
	{
		observed.increaseRow();
		observed.increaseColumn();
	}
	// Iterate through and increment by the given correct value
	for(int i= 0; i< observation.width(); i++)
	{
		int	x=	mapPercentToIndex(observation.getCell(i, 0));
		int	y=	mapPercentToIndex(observation.getCell(i, 1));
		
		observed.setCell(x, y, observed.getCell(x, y)+1);
	}
	
	return observed;
}

 int mapPercentToIndex(float percent)
 {
 	if(percent<= 0)
 		return 0;
 	
 	int	n=	(int)(100*percent);
 	float	d=	100*percent-n;
 	
 	if(d!= 0 && d<= 0.5)
 		return 2*n+1;
 	else
 		return 2*n;
 }








template<class T>
 int sumCol(vector< SetList<T> > Table, int col) ;  // protype for summing the cols 
//******************************************************************

template <class T>
int SetList<T>::sum()
{ 
   int sum = 0;
   nodePtr temp = head;
   while (temp != NULL) {
     sum += temp->data; 
     temp = temp->next;
   }
   return sum;
}
//********************************************************************************
template<class T>
int sumCol(vector< SetList<T> > Table, int col) 
{
	int total = 0;
	for( int i =0 ;i< Table.size(); i++)
	{
		total+= Table[i].at(col);		
	}
	return total;
}
//********************************************************************************

//*******************************************************************
template < class T >
void SetList<T>:: printList() {
	int i = 0;
        nodePtr current = head;
        while (current != NULL) {
            cout << setw(6)<<current->data;
            current = current->next;
            i++;
        }	
        //cout << "\n";
}
//********************************************************************************
template <class T>
void SetList <T>::appendElement(T Nvalue)
{
    element *n = new element{ Nvalue, NULL };
    if (head == NULL)
        head = n;
    else
    {
        element *p = head;
        while (p->next)
            p = p->next;
        p->next = n;
    }
}
//********************************************************************************
template <class T>
void SetList<T>::removeElement(int i) { // search data
    element *elementptr;
    element *turtptr;
    if (!head)
        return;
    if (i== 0) {
        elementptr = head->next;     //the next node will be the current node
        delete head;
        head = elementptr;
    }
    else
    {
        elementptr = head;
        while (elementptr != 0 && i!= 0)
        {
    		i--;
            turtptr = elementptr;
            elementptr = elementptr->next;
        }
        if (elementptr) {
            turtptr->next = elementptr->next;
            delete elementptr;
        }
    }
}
//********************************************************************************
template <class T>
bool SetList <T> ::findElement(T Svalue) { // search data
    element *elementptr;
    if (!head)
        return false;
    else
    {
        elementptr = head;
        int count = 0;
        while (elementptr != 0 && elementptr->data != Svalue)
        {
            count++;
            elementptr = elementptr->next;
        }
        if (elementptr) {
            cout << "Found at index " << count << " ";
            return true;
        }
    }
    return false;
}
//********************************************************************************
template <class T>
void SetList<T>::clearSetList() {
    element *elementptr;
    element    *frogptr;
    elementptr = head;
    while (elementptr != 0) {
        frogptr = elementptr->next;
        delete elementptr;
        elementptr = frogptr;
    }
}
//********************************************************************************
template <class T>
void SetList<T>::printSetList() {
    element *elementptr;
    elementptr = head;
    while (elementptr != 0) {
        cout << elementptr->data << "\t";
        elementptr = elementptr->next;
    }
    cout << endl;
}
//********************************************************************************
template <class T>
void SetList<T>::partition(nodePtr head, nodePtr *front, nodePtr *back) {
    nodePtr fast;
    nodePtr slow;
    if (head == NULL || head->next == NULL) {
        *front = head; // &a
        *back = NULL; // &b
    }
    else {
        slow = head;
        fast = head->next;
        while (fast != NULL) {
            fast = fast->next;
            if (fast != NULL) {
                slow = slow->next;
                fast = fast->next;
            }
        }
        *front = head; // a
        *back = slow->next; // b
        slow->next = NULL;
        // printList(*front);
        //printList(*back);
    }
}
//********************************************************************************
template <class T>
void SetList<T>::mergeSort(nodePtr *source) {
    nodePtr head = *source;
    nodePtr a = NULL;
    nodePtr b = NULL;
    if (head == NULL || head->next == NULL) {
        return;
    }
    partition(head, &a, &b);
    mergeSort(&a);
    mergeSort(&b);
    *source = mergeLists(a, b);
}

//********************************************************************************
template <class T>
T& SetList<T>::at(int i)
{
    int pos = 0;
    if (i < 0)
        throw "OUT OF BOUNDS";
    element *p = head;
    while (p && pos < i)
    {
        p = p->next;
        pos++;
    }
    if (!p)
        throw "OUT OF BOUNDS";
    return p->data;
}
//********************************************************************************
template <class T>
int SetList<T>::size() const
{
    int count = 0;
    element *p = head;
    while (p)
    {
        count++;
        p = p->next;
    }
    return count;
}
//********************************************************************************
template <class T>
void SetList<T>::pop_back()
{
    if (head != NULL)
    {
        element *p = head, *q = NULL;
        // advance p an q to where they need to be
        while (p->next)
        {
            q = p;
            p = p->next;
        }
        // p points at the last node in the list by now
        delete p;
        // if q isn't nullptr, then it's one node behind p

        // and after deleting p, q points to the last

        // node, so we need to set it's next to nullptr.
        if (q)
            q->next = NULL;
        // if p never moved, then there was only one node in

        // the list, so head node was deleted, so we need

        // to set head back to nullptr.

        if (p == head)
            head = NULL;
    }
}
//********************************************************************************
template <class T>
void SetList<T>::insert(int pos, int v)
{
    element * newnode = (element *)malloc(sizeof(element));
    newnode->data = v;
    element *temp = head, *tail;
    if (pos == 0) {
        newnode->next = head;
        head = newnode;
    }
    else {
        while (pos--) 
		{
            tail = temp;
            temp = temp->next;
        }
        tail->next = newnode;
        newnode->next = temp;
    }
}
//********************************************************************************
//-----------------------------hash table section BEGINS-----------------------
int main() 
{
	BivariateTable	table=	BivariateTable();
	
	readFromCSV(table, "CVS/TEST1.csv");
	writeToCSVExtra(table, "CVS/OUTPUT-bivariate-table.csv");
	table.createProbability();
	table.display();
	writeToCSV(table, "CVS/OUTPUT-probability-table.csv");
	
	// Testing theEye
	BivariateTable	observation=	BivariateTable();
	readFromCSV(observation, "CVS/Observation.csv");
	BivariateTable	observed=	theEye(observation);
	writeToCSV(observed, "CVS/Observed.csv");






	// A.mergeSort();
  //   A.printSetList();*/
//********************************************************************************
    return 0;

}



