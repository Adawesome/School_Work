#include "stdio.h"
#include "stdlib.h"
struct Employee {
	void** vtable;
	int age;
};
struct HourlyEmployee {
	void** vtable;
	int age;
	double hourly_rate;
	double hours;
};
struct CommissionEmployee { 
	void** vtable;
	int age;
	double sales_amount;
};
struct SeniorSalesman {
	void** vtable;
	int age;
	double sales_amount;
};
void Speak_Hourly(struct Employee* emp, double getpay) {
	//struct HourlyEmployee* he = (struct Employee*)emp;
	printf("I am %d years old\n", emp->age);
	printf("I make $%lf in total\n", getpay);

}
void Speak_Commission(struct Employee* emp, double paid) {
	//struct CommissionEmployee* ce = (struct Employee*)emp;
	printf("I am %d years old\n", emp->age);
	printf("My sales Commission is %lf\n", paid);
}
void Speak_SeniorSalesman(struct Employee* emp, double pay) {
	//struct SeniorSalesman* se = (struct Employee*)emp;
	printf("I am %d years old\n", emp->age);
	printf("I have made %lf sales.\n", pay);
}
double GetPay_Hourly(struct Employee* emp) {
	double earnings = ((struct HourlyEmployee*)emp)->hourly_rate*((struct HourlyEmployee*)emp)->hours;
	return earnings;
}
double GetPay_Commission(struct Employee* emp) {
	return ((struct CommissionEmployee*)emp)->sales_amount*.1 + 40000;
}
double GetPay_SeniorSalesman(struct Employee* emp) {
	struct SeniorSalesman* sales = (struct Employee*)emp;
	if (sales->age >= 40)
		return sales->sales_amount*.25 + 50000;
	else
		return sales->sales_amount*.2 + 50000;
}
void* Vtable_Hourly[2] = { Speak_Hourly, GetPay_Hourly };
void* Vtable_Commission[2] = { Speak_Commission,GetPay_Commission };
void* Vtable_SeniorSalesman[2] = { Speak_Commission, GetPay_SeniorSalesman };
void Construct_Hourly(struct HourlyEmployee* he) {
	he->age = 0;
	he->hourly_rate = 0;
	he->hours = 0;
	he->vtable = Vtable_Hourly;
}
void Construct_Commission(struct CommissionEmployee* ce) {
	ce->age = 0;
	ce->sales_amount = 0;
	ce->vtable = Vtable_Commission;
}

void Construct_SeniorSalesman(struct SeniorSalesman* sse) {
	sse->age = 0;
	sse->sales_amount = 0;
	sse->vtable = Vtable_SeniorSalesman;
}

int main(void) {
	struct Employee* worker;
	int empType;
	int empAge;
	printf("Select which type of Employee you want to create: \n");
	printf("(1) Hourly Employee\n");
	printf("(2) Commission Employee\n");
	printf("(3) Senior Salesman\n");
	scanf_s("%d", &empType);
	printf("Please enter the age of the Employee: ");
	scanf_s("%d", &empAge);
	if (empType == 1) {
		//Hourly Employee Configure
		worker = malloc(sizeof(struct HourlyEmployee));
		double pay = 0;
		double hours = 0;
		printf("Please enter Hourly_Employee's pay rate and hours: \n");
		printf("Pay: ");
		scanf_s("%lf", &pay);
		printf("\n");
		printf("Hours: ");
		scanf_s("%lf", &hours);
		//start init worker
		Construct_Hourly(worker);
		worker->age = empAge;
		((struct HourlyEmployee*)worker)->hours = hours;
		((struct HourlyEmployee*)worker)->hourly_rate = pay;
		double paid = ((double(*)(struct Employee*))Vtable_Hourly[1])((struct Employee*)worker);
		((void(*)(struct Employee*))Vtable_Hourly[0])((struct Employee*)worker, paid);
		// V console terminates without this here V
		scanf_s("%d", empAge);
	}
	else if (empType == 2) {
		//Commission Employee configure
		worker = malloc(sizeof(struct CommissionEmployee));
		double sales = 0;
		printf("Please enter Commission_Employee's sales_amount: ");
		scanf_s("%lf", &sales);
		Construct_Commission(worker);
		worker->age = empAge;
		((struct CommissionEmployee*)worker)->sales_amount = sales;
		double getpay = ((double(*)(struct Employee*))Vtable_Commission[1])((struct Employee*)worker);
		((void(*)(struct Employee*))Vtable_Commission[0])((struct Employee*)worker, getpay);
		// V console terminates w/o below statement V
		scanf_s("%d", empAge);
	}
	else if (empType == 3) {
		//Senior Salesman Configure
		worker = malloc(sizeof(struct SeniorSalesman));
		double sales = 0;
		printf("Please enter Commission_Employee's sales_amount: ");
		scanf_s("%lf", &sales);
		Construct_SeniorSalesman(worker);
		worker->age = empAge;
		((struct CommissionEmployee*)worker)->sales_amount = sales;
		double pay = ((double(*)(struct Employee*))Vtable_SeniorSalesman[1])((struct Employee*)worker);
		((void(*)(struct Employee*))Vtable_SeniorSalesman[0])((struct Employee*)worker, pay);
		// V console terminates without below statement V
		scanf_s("%d", empAge);
	}
	else
		worker = malloc(sizeof(struct Employee)); //secure init pointer
	free(worker);
	return 0;
}