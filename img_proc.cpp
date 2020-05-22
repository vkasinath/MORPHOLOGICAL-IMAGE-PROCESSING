#include <string>
#include <fstream>
#include <iostream>
#include <cctype>
#include <vector>

using namespace std;

/* ***************************************************
 * utility function: check for escape character
 * if first character in the string is \
 * then return the second character in the string
 * else return the first character in the string
 * ***************************************************
 */
char check_for_escape(string &x){

	char s_str;

	if (x.size() == 1)
		s_str = x[0];
	else{
		if (x.substr(0,1) == "\\")
			s_str = x[1];
		else
			s_str = x[0];
	}

	return s_str;
}


/* ***************************************************
 * replace operation
 * loop through input string one character at a time.
 * if the char equals c_orig, change it to c_new.
 * return the modifed string vector
 * ***************************************************
*/
vector <string> replace(vector <string> data, char* c_orig, char* c_new){

	vector <string> replaced_data;

	/* Read each row of data (the i loop) -> new_row
	 * read each char in the row (the j loop)
	 * if (i,j) char = c_orig, replace it with c_new and push this new c_new char to output vector
	 * else push the (i,j) char as is
	 */

	for (unsigned int i = 0; i < data.size(); i++){ 
		string new_row = "";

		for (unsigned int j = 0; j < data[i].size(); j++){
			if (data[i][j] == *c_orig)
				new_row.push_back(*c_new);
			else
				new_row.push_back(data[i][j]);
		}

		replaced_data.push_back(new_row);
		//cout << new_row << endl;
	}

	return replaced_data;
}


/* *******************************************************************
 * erosion operation
 * loop through input string one line at a time.
 * check each of the cell in "+" around each point (i,j)
 * if all the cells in the "+" around a point = source character, then
 * keep the (i,j) cell as the source character
 * else push the (i,j) cell to be the target character
 * return the modifed string
 *********************************************************************
*/

vector <string> erosion(vector <string> data, char* c_orig, char* c_new){

vector <string> eroded_data;

	int r = data.size() - 1; // index of rows

	/* Read each row of data (the i loop) -> new_row
	 * read each char in the row (the j loop)
	 * perform erosion for each (i,j) char based on + rules
	 * push the modified (i,j) char back to the output vector string
	 */

	for (int i = 0; i < static_cast<int>(data.size()); i++){

		string new_row = "";
		int c = data[i].size() - 1; // index of columns for row i

		for (int j = 0; j < static_cast<int>(data[i].size()); j++){

			// x and y values of the cells that form a + around the point (i,j) are
			// the 4 cells (i-1,j), (i+1,j), (i,j-1), and (i,j+1)
			// to keep ourselves within the boundaries of the grid
			// we constrain our + cells (top, bottom, left, right) like so -
			// x value of the top cell    = max(0, i-1), so lowerbound of x is restricted 0
			// x value of the bottom cell = min(i+1,r), so upperbound of x is restricted to r; where r = index of last row
			// y value of the left cell   = max(j-1, 0), so left-sidebound of y is restricted to 0
			// y value of the right cell  = min(j+1,c), so left-sidebound of y is restricted to c; where c = index of rightmost column


			int x1 = max(0, i-1);
			int x2 = min(i+1, r);
			int y1 = max(j-1, 0);
			int y2 = min(j+1, c);

			if ( ( (data[x1][j] == *c_orig) && 
				   (data[x2][j] == *c_orig) && 
				   (data[i][y1] == *c_orig) && 
				   (data[i][y2] == *c_orig)
				  ) && ((x1 != i) 
				    && (x2 != i) 
				    && (y1 != j) 
				    && (y2 != j))
				)
				new_row.push_back(data[i][j]);
			else
				new_row.push_back(*c_new);
		}

		eroded_data.push_back(new_row);
	}

	return eroded_data;
}


/* **********************************************************************
 * dilation operation
 * loop through input string one line at a time.
 * check each of the cell in "+" around each point (i,j)
 * if any of the cells in the "+" around a point = source character, then
 * push the (i,j) cell to be the source character
 * else leave the (i,j) cell as is
 * return the modifed string
 ************************************************************************
*/

vector <string> dilation(vector <string> data, char* c_orig){

	vector <string> dilated_data;

	int r = data.size() - 1; // max index of row

	/* Read each row of data (the i loop) -> new_row
	 * read each char in the row (the j loop)
	 * perform dilation for each (i,j) char based on + rules
	 * push the modified (i,j) char back to the output vector string
	 */

	for (int i = 0; i < static_cast<int>(data.size()); i++){

		string new_row = "";
		int c = data[i].size() - 1; // max index of column for row i

		for (int j = 0; j < static_cast<int>(data[i].size()); j++){

			// x and y values of the cells that form a + around the point (i,j) are
			// the 4 cells (i-1,j), (i+1,j), (i,j-1), and (i,j+1)
			// to keep ourselves within the boundaries of the grid
			// we constrain our + cells (top, bottom, left, right) like so -
			// x value of the top cell    = max(0, i-1), so lowerbound of x is restricted 0
			// x value of the bottom cell = min(i+1,r), so upperbound of x is restricted to r; where r = index of last row
			// y value of the left cell   = max(j-1, 0), so left-sidebound of y is restricted to 0
			// y value of the right cell  = min(j+1,c), so left-sidebound of y is restricted to c; where c = index of rightmost column

			int x1 = max(0, i-1);
			int x2 = min(i+1, r);
			int y1 = max(j-1, 0);
			int y2 = min(j+1, c);

			if ( ( (data[x1][j] == *c_orig) || 
				   (data[x2][j] == *c_orig) || 
				   (data[i][y1] == *c_orig) || 
				   (data[i][y2] == *c_orig)
				  ) && ((x1 != i) 
				    && (x2 != i) 
				    && (y1 != j) 
				    && (y2 != j))
				)
				new_row.push_back(*c_orig);
			else
				new_row.push_back(data[i][j]);
		}

		dilated_data.push_back(new_row);
	}

	return dilated_data;
}

/* Main Function */
int main( int argc, char* argv[]) {

	//First check for valid number of arguments
	//We should have a minmum of 5 (sourcefilename + 4 others... depending on operation)
	//and a maximum of 6 (sourcefilename + 5 others... depending on operation)

if (argc < 5) {
	cerr << "not enough input arguments";
	return 1;
}

else if (argc > 6) {
	cerr << "too many input arguments";
	return 2;
}


	// At this point, we should have either 4 or 5 arguments, other than the source filename
	// The first argument is the input file name
	// check if the file exists. if file does not exist, throw an error and stop
string InFileName;
InFileName = argv[1];
ifstream f(InFileName.c_str());
if (!f.good()){
	cerr << "error opening input file " + InFileName;
	return 3;
}

// the second argument is the output file. we have to create the file
	// at this time, not really much error checking to do for the second argument
string OutFileName = argv[2];


	// the third argument should be one of the following
	// replace, dilation, erosion, opening, closing
	// if the third argument is not one of these 5, throw an error
const string C_REPLACE = "replace";
const string C_DILATION = "dilation";
const string C_EROSION = "erosion";
const string C_OPENING = "opening";
const string C_CLOSING = "closing";

string operation;
operation = argv[3];

if ( (operation != C_REPLACE) && 
	 (operation != C_DILATION) && 
	 (operation != C_EROSION) && 
	 (operation != C_OPENING) && 
	 (operation != C_CLOSING)){
	cerr << "invalid file operation: " + operation;

	return 4;
}

	// if here, we have a valid operation to perform on the input file
	// for each operation, we nd different inputs, like so -
	// for replace need the source string, and the target string (arguments 4 and 5)
	// for erosion need the source string, and the target string (arguments 4 and 5)
	// if we do not have the 4th and 5th argument, error out.
	// else set source char c_orig to argv4 and target char c_new to argv5
	//

char c_orig;
char c_new;

if ( (operation == C_REPLACE) || 
	 (operation == C_EROSION) || 
	 (operation == C_OPENING) || 
	 (operation == C_CLOSING)){
	if (argc < 6){
		cerr << "need original and new character for operation " + operation;

		return 5;
	}

	else{

		string x = argv[4];
		c_orig = check_for_escape(x);

		x = argv[5];
		c_new = check_for_escape(x);
	}
}

else if (operation == C_DILATION){
	string x = argv[4];
	c_orig = check_for_escape(x);
}

	// if we are here, we have a valid file, operation and the arguments required
	// Read the file and push_back each line into out vector string variable = data

string line;
vector <string> data;
while (getline(f, line)){
	data.push_back(line);
	//cout << line << endl;
}

vector <string> new_data;

if (operation == C_REPLACE){
	new_data = replace(data, &c_orig, &c_new);
}

else if (operation == C_EROSION){
	new_data = erosion(data, &c_orig, &c_new);
}

else if (operation == C_DILATION){
	new_data = dilation(data, &c_orig);
}

else if (operation == C_OPENING){

		/* OPENING = apply erosion -> the output will in the temp vector out_ero.
		 * Feed out_ero to dilation for final result */

	vector <string> temp = erosion(data, &c_orig, &c_new);
	new_data = dilation(temp, &c_orig);
}

else if (operation == C_CLOSING){

		/* CLOSING = apply dilation -> the output will in the temp vector out_dil.
		 * Feed out_dil to erosion for final result */

	vector <string> temp = dilation(data, &c_orig);
	new_data = erosion(temp, &c_orig, &c_new)	;
}

	// the output data is in the vector new_data
	// write it out to the output file OutFileName

ofstream out_file;
out_file.open(OutFileName.c_str());
for (unsigned int i = 0; i < new_data.size(); i++){
	out_file << new_data[i] << endl;
}
	out_file.close();

return 0;
}