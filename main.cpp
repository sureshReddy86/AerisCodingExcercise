#include <iostream>
#include<vector>
#include<ctime>
#include <bits/stdc++.h>
#include<string>
#include<sstream>
#include<map>
using namespace std;



void DatePlusDays( struct tm* date, int days )
{
    const time_t ONE_DAY = 24 * 60 * 60 ;
    time_t date_seconds = mktime( date ) + (days * ONE_DAY) ;
    *date = *localtime( &date_seconds ) ;
}

int stringToInteger(string s){
    stringstream temp(s);
    int x = 0;
    temp >> x;
    return x;
}

string toString(int i){
    ostringstream str1;
    str1 << i;
    return str1.str();
}

string dateToString(struct tm date){
    string year =  toString(date.tm_year+1900);
    string month = toString(date.tm_mon+1);
    if(month.length()==1){
        month = "0"+month;
    }
    string day = toString(date.tm_mday);
    if(day.length()==1){
        day = "0"+day;
    }
    return year+"-"+month+"-"+day;
}

string calculateDatePlusDays(string startDate, int days){

    vector <string> tokens;
    stringstream check1(startDate);
    string intermediate;
    while(getline(check1, intermediate, '-'))
    {
        tokens.push_back(intermediate);
    }

    struct tm date = { 0, 0, 12 } ;
    int year = stringToInteger(tokens[0]) ;
    int month = stringToInteger(tokens[1]) ;
    int day = stringToInteger(tokens[2]) ;

    date.tm_year = year - 1900 ;
    date.tm_mon = month - 1 ;
    date.tm_mday = day ;

    DatePlusDays( &date, days ) ;

    return dateToString(date);
}


class Product {
    string customerId, productName, domain, startDate,  expiryDate;
    int duration;
protected:
    vector<string> notificationDates;
    void setActivationDays(int a[], int n){
        for(int i=0;i<n;i++){
            notificationDates.push_back(calculateDatePlusDays(this->startDate, a[i]));
        }
    }
    void setExpirationDays(int a[], int n){
        for(int i=0;i<n;i++){
            notificationDates.push_back(calculateDatePlusDays(this->expiryDate, a[i]));
        }
    }
public:
    Product(string customerId,string productName,string domain,string startDate,int duration){
        this->customerId = customerId;
        this->productName = productName;
        this->domain = domain;
        this->startDate = startDate;
        this->duration = duration;
        this->expiryDate = calculateDatePlusDays(startDate, duration*30);
    }
    static Product* Create(string customerId,string productName,string domain,string startDate,int duration);
    friend void ListDownNotificationUtil(vector<Product *>);
    friend bool IsMatching(string, string, string, Product *);

};


class Hosting : public Product {

private:
    int activation[1] = {1};
    int expiration[1] = {-3};

public:
    Hosting(string customerId,string productName,string domain,string startDate,int duration):Product(customerId,productName,domain,startDate,duration) {
        if(sizeof(activation)) setActivationDays(activation, sizeof(activation)/sizeof(activation[0]));
        if(sizeof(expiration)) setExpirationDays(expiration, sizeof(expiration)/sizeof(expiration[0]));
    }
};


class Domain : public Product {

private:
    int activation[] = {};
    int expiration[1] = {-2};

public:
    Domain(string customerId,string productName,string domain,string startDate,int duration):Product(customerId,productName,domain,startDate,duration) {
        if(sizeof(activation)) setActivationDays(activation, sizeof(activation)/sizeof(activation[0]));
        if(sizeof(expiration)) setExpirationDays(expiration, sizeof(expiration)/sizeof(expiration[0]));
    }
};


class Pdomain : public Product {

private:
    int activation[] = {};
    int expiration[2] = {-9, -2};

public:
    Pdomain(string customerId,string productName,string domain,string startDate,int duration):Product(customerId,productName,domain,startDate,duration) {
        if(sizeof(activation)) setActivationDays(activation, sizeof(activation)/sizeof(activation[0]));
        if(sizeof(expiration)) setExpirationDays(expiration, sizeof(expiration)/sizeof(expiration[0]));
    }
};

Product* Product::Create(string customerId,string productName,string domain,string startDate,int duration) {
    if (productName == "hosting")
        return new Hosting(customerId,productName,domain,startDate,duration);
    else if (productName == "domain")
        return new Domain(customerId,productName,domain,startDate,duration);
    else if (productName == "pdomain")
        return new Pdomain(customerId,productName,domain,startDate,duration);

    else return NULL;
}


class Client {
public:

    Client(string customerId,string productName,string domain,string startDate,int duration)
    {
        pProduct = Product::Create(customerId,productName,domain,startDate,duration);
    }
    ~Client() {
        if (pProduct) {
            delete[] pProduct;
            pProduct = NULL;
        }
    }
    Product* getProduct()  {
        return pProduct;
    }
    friend void ListDownNotification(vector<Client *>);
    friend void DeleteProduct(string, string, string, vector<Client *>&);

private:
    Product *pProduct;
};

void ListDownNotificationUtil(vector<Product *> products){
    map<string, Product*> tempMap;
    for(int i=0;i<products.size();i++)
    {
        for(int j=0;j<products[i]->notificationDates.size();j++){
            string key = products[i]->notificationDates[j]+" "+products[i]->customerId+" "+products[i]->domain+" "+products[i]->productName;
            tempMap[key] = products[i];
        }
    }
    cout<<"\n";
    cout <<"CustomerID     "
         <<"Product Name   "
         <<"Domain         "
         <<"EmailDate      "
         << "\n";
    map<string, Product*>::iterator itr;
    for (itr = tempMap.begin(); itr != tempMap.end(); ++itr) {
        cout <<itr->second->customerId
             << "\t  " << itr->second->productName
             << "\t " << itr->second->domain
             << "\t" << itr->first.substr(0,10) << '\n';
    }
}

void ListDownNotification(vector<Client *> clientObjects){
    vector<Product *> products;
    for(int i=0;i<clientObjects.size();i++)
    {
        products.push_back(clientObjects[i]->getProduct());
    }
    ListDownNotificationUtil(products);

}

bool IsMatching(string customerId, string productName, string domain, Product *product){
    return (product->customerId == customerId && product->productName == productName && product->domain == domain);

}

void DeleteProduct(string customerId, string productName, string domain, vector<Client *> &clientObjects){
    vector<Client *>::iterator it;
    for(it = clientObjects.begin(); it != clientObjects.end(); ++it){
        Product *product = (*it)->getProduct();
        if(IsMatching(customerId, productName, domain, product)){
            clientObjects.erase(it);
        }
    }

}

void EnterProductMain(vector<Client *> &clientObjects){
    string customerId, productName, domain, startDate;
    int duration;
    cout<<"Enter customer id : ";
    cin>>customerId;
    cout<<"Enter product name : ";
    cin>>productName;
    cout<<"Enter domain : ";
    cin>>domain;
    cout<<"Enter start date (YYYY-MM-DD) : ";
    cin>>startDate;
    cout<<"Enter duration (in months) : ";
    cin>>duration;

    Client *pClient = new Client(customerId, productName, domain, startDate, duration);
    clientObjects.push_back(pClient);

}

void DeleteProductMain(vector<Client *> &clientObjects){
    string customerId, productName, domain;
    cout<<"Enter customer id : ";
    cin>>customerId;
    cout<<"Enter product name : ";
    cin>>productName;
    cout<<"Enter domain : ";
    cin>>domain;
    vector<Client *>::iterator it;
    for(it = clientObjects.begin(); it != clientObjects.end(); ++it){
        Product *product = (*it)->getProduct();
        if(IsMatching(customerId, productName, domain, product)){
            clientObjects.erase(it);
            break;
        }
    }
}


// Driver program
int main() {
    int ch;
    vector<Client *> clients;
    while(1){
        cout<<"1. Enter Product\n";
        cout<<"2. Delete Product\n";
        cout<<"3. List Notifications\n";
        cout<<"Enter your choice : ";
        cin>>ch;
        if(ch<1 || ch>3) break;
        switch(ch){
            case 1: EnterProductMain(clients);
                    break;
            case 2: DeleteProductMain(clients);
                    break;
            case 3 : ListDownNotification(clients);
                    break;
            default: break;
        }
    }

    return 0;
}
