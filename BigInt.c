#include<stdio.h>
#define SIZE 310
typedef enum{PLUS,MINUS} signBit;
typedef struct BigInt_tag
{
    char digit[SIZE];
    signBit sign;
} BigInt;


void printBigInt(BigInt* num);
void initializeBigInt(BigInt* num);
void  setBigInt(char numString[], BigInt* num);
BigInt add(BigInt num1, BigInt num2);
BigInt sub(BigInt num1, BigInt num2);
BigInt multi(BigInt num1, BigInt num2);

//helper functions
BigInt addPlus(BigInt num1, BigInt num2);
BigInt subPlus(BigInt num1, BigInt num2);
int numlen(BigInt* num);

int main()
{
	int t;
	BigInt test1,test2,ans;
    initializeBigInt(&test1);
    initializeBigInt(&test2);
    initializeBigInt(&ans);
    char NumString1[SIZE],NumString2[SIZE];
    printf("Enter number of test cases : ");
    scanf("%d",&t);
    while(t--) 
	{
		initializeBigInt(&test1);
    	initializeBigInt(&test2);
    	initializeBigInt(&ans);
    	
    	printf("Enter num1 : ");
		scanf("%s",NumString1);
		printf("Enter num2 : ");
		scanf("%s",NumString2);
		
		setBigInt(NumString1,&test1);
    	setBigInt(NumString2,&test2);
		
		ans = add(test1,test2);
    	printf("Addition is ");	printBigInt(&ans);	printf("\n");
    	
    	ans = sub(test1,test2);
    	printf("Subtraction is ");	printBigInt(&ans);	printf("\n");
    	
    	ans = multi(test1,test2);
    	printf("Multiplication is ");	printBigInt(&ans);	printf("\n");

		printf("\n");
    }

    return 0;
}

//Function to initialize an object, of BigInt data type
void initializeBigInt(BigInt* big)
{
    int i;
    big->sign=PLUS;
    for(i=0;i<SIZE;i++)
    {
        big->digit[i]='0';
    }
}

//Function to validate the input string and store it in the object big, of BigInt data type
void setBigInt(char numString[],BigInt* big)
{
	int i,j;
	i=0;
	j=0;
	
	while(numString[i] != '\0')i++;	//Number is stored such a way, LSD is at 0th index, and MSD at last index of char array.
	
	i--;	//Index of last digit(unit's digit)
	while(i>=1)		//Storage similar to little endian
	{
		big->digit[j] = numString[i];
		i--;j++;
	}
	
	if(numString[0] == '-')
	{
		big->sign=MINUS;
	}
	else if(numString[0] == '+')
	{
		big->sign=PLUS;
	}
	else
	{
		big->sign=PLUS;
		big->digit[j]=numString[0];
	}
		
}

//Function to print an object, of BigInt data type
void printBigInt(BigInt* big)
{
    int i=SIZE-1;
    while(i>=0 && big->digit[i] == '0')
    	i--;
    
    if(i == -1)
    	printf("0");
    else
    {
    	if(big->sign == MINUS)
    	{
			printf("-");
    	}				//Number is stored such a way, LSD is at 0th index, and MSD at last index of char array.
    	while(i>=0)		//e.g. num stored as 123000...000, it will be printed as 321, ignoring all the ending zeroes.
    	{
    		printf("%c",big->digit[i]);
    		i--;
		}
	}
}

//Function for addition of 2 BigInt data type numbers. ( (-inf < num1 < inf) && (-inf < num2 < inf) ) 
//Returns ans = num1 + num2
BigInt add(BigInt num1,BigInt num2)
{
	BigInt ans;
	initializeBigInt(&ans);
	
	if(num1.sign == PLUS && num2.sign == PLUS)
	{
		ans = addPlus(num1,num2);
	}
	else if(num1.sign == PLUS && num2.sign == MINUS)
	{
		num2.sign = PLUS;
		ans = subPlus(num1,num2);
	}
	else if(num1.sign == MINUS && num2.sign == PLUS)
	{
		num1.sign = PLUS;
		ans = subPlus(num2,num1);
	}
	else	//num1.sign == MINUS && num2.sign == MINUS
	{
		num1.sign = PLUS;
		num2.sign = PLUS;
		ans = addPlus(num1,num2);
		ans.sign = MINUS;
	}
	
	return ans;
}

//Function for subtraction of 2 BigInt data type numbers. (-inf < num1 < inf && -inf < num2 < inf) 
//Returns ans = num1 - num2
BigInt sub(BigInt num1,BigInt num2)
{
	BigInt ans;
	initializeBigInt(&ans);
	
	if(num1.sign == PLUS && num2.sign == PLUS)
	{
		ans = subPlus(num1,num2);
	}
	else if(num1.sign == PLUS && num2.sign == MINUS)
	{
		num2.sign = PLUS;
		ans = addPlus(num1,num2);
	}
	else if(num1.sign == MINUS && num2.sign == PLUS)
	{
		num1.sign = PLUS;
		ans = addPlus(num1,num2);
		ans.sign = MINUS;
	}
	else	//num1.sign == MINUS && num2.sign == MINUS
	{
		num2.sign = PLUS;
		num1.sign = PLUS;
		ans = subPlus(num2,num1);
	}
	
	return ans;
}

//Function for addition of 2 positive BigInt data type numbers. (num1 > 0 && num2 > 0) 
//Returns ans = num1 + num2
BigInt addPlus(BigInt num1,BigInt num2)
{
	BigInt ans;
	initializeBigInt(&ans);
	int i;
	char carry;
	i=0;
	carry='0';
	
	while(i<SIZE)
	{
		ans.digit[i] = ( ( num1.digit[i]+num2.digit[i]+carry- (3*'0') )%10 ) + '0';
		carry = ( ( num1.digit[i]+num2.digit[i]+carry- (3*'0') )/10 ) + '0';
		i++;
	}

	return ans;
}

//Function for subtraction of 2 positive BigInt numbers. (num1 > 0 && num2 > 0) 
//Returns ans = num1 - num2
BigInt subPlus(BigInt num1,BigInt num2)
{
	BigInt ans;
	initializeBigInt(&ans);
	int i;
	char carry,tempDigit;
	
	i=0;
	carry='0';
	
	while(i<SIZE)
	{
		num2.digit[i] = ( '9' - num2.digit[i] ) + '0';
		i++;
	}

	i=0;
	while(i<SIZE)	//Adding num1 with 9's complement of num2
	{
		ans.digit[i] = ( ( num1.digit[i]+num2.digit[i]+carry- (3*'0') )%10 ) + '0';
		carry = ( ( num1.digit[i]+num2.digit[i]+carry- (3*'0') )/10 ) + '0';
		i++;
	}
	
	if(carry == '0')	//If no carry, ans is negative and apply 9's complement to ans again
	{
		if(numlen(&ans) == 1 && ans.digit[0] == '0')	//If answer is 0, setting its sign +
		{
			ans.sign = PLUS;
		}
		else
		{
			ans.sign = MINUS;
		}
		
		i=0;
		while(i<SIZE)
		{
			ans.digit[i] = ( '9' - ans.digit[i] ) + '0';
			i++;
		}
	}
	else	//If carry, ans is positive, and add the carry to least significant digit of ans
	{
		ans.sign = PLUS;
		i=0;
		while(carry != '0' && i < SIZE)
		{
			tempDigit = ans.digit[i];	//Storing old value of digit temporarily
			ans.digit[i] = ( (tempDigit - '0') + (carry - '0') )%10 + '0';
			carry = ( (tempDigit - '0') + (carry - '0') )/10 + '0';
			i++;
		}
	}
	
	return ans;
}

//Function that calculates the number of digits of a BigInt data type number
int numlen(BigInt* num)
{
	int i = SIZE-1;
	while(i>=0 && num->digit[i] == '0')
		i--;
	
	if(i == -1)	//If the number is zero, then its length is 1
		i=0;
	
	return i+1;
}

//Function for multiplication of 2 BigInt data type numbers. ( (-inf < num1 < inf) && (-inf < num2 < inf) ) 
//Returns ans = num1 * num2
/*
Ex:
        4 5 3 6
    x       5 6
        --------
      2 7 2 1 6
    2 2 6 8 0
----------------
    2 5 4 0 1 6
*/
BigInt multi(BigInt num1,BigInt num2)
{
	BigInt ans;
	initializeBigInt(&ans);
	char carry,tempDigit;
	int i,j,len1,len2;
	
	len1=numlen(&num1);
	len2=numlen(&num2);
		
	if((num1.sign == PLUS && num2.sign == MINUS) || (num1.sign == MINUS && num2.sign == PLUS))
	{
		ans.sign = MINUS;
	}
	else if((num1.sign == PLUS && num2.sign == PLUS) || (num1.sign == MINUS && num2.sign == MINUS))
	{
		ans.sign = PLUS;
	}
	
	for(j=0;j<len2;j++)
	{
		carry='0';
		for(i=0;i<len1;i++)
		{
			tempDigit = ans.digit[j+i];	//Storing old value of digit temporarily
			ans.digit[j+i] = ( ( (num1.digit[i]-'0')*(num2.digit[j]-'0') ) + (carry-'0') + (tempDigit-'0') )%10 + '0';
			carry = ( ( (num1.digit[i]-'0')*(num2.digit[j]-'0') ) + (carry-'0') + (tempDigit-'0') )/10 + '0';
		}
		ans.digit[j+i] = carry;
	}
	
	return ans;
}
