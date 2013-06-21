struct TempStruct 
{ 
	int array1[100]; 
	float array2[10][10]; 
}; 

struct TempStruct2 
{ 
	int array3[100]; 
	float array4[10][10]; 
};

int main()
{
	struct TempStruct t1;
	struct TempStruct2 t2;
	t1 = t2;
}
