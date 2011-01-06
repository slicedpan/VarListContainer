/* 
 * File:   varlist.h
 * Author: Owen
 *
 * Created on 05 December 2010, 14:45
 */

#ifndef _VARLIST_H
#define	_VARLIST_H

#include <vector>
#include <string>
#include <stdexcept>

namespace VarList
{

class Exception : public std::exception
{
public:
	virtual const char * what()
	{
		return msg.c_str();
	}
	Exception() : msg("Default Exception") {}
	Exception(const char * p_msg) : msg(p_msg) {}
	virtual ~Exception() throw() {}
private:
	std::string msg;
};

class VarNotFoundException : public Exception
{
public:
	VarNotFoundException() : msg("Variable not found"){}
	VarNotFoundException(std::string& varname)
	{
		msg = "Variable: \"" + varname + "\" not found";
	}
	const char * what()
	{
		return msg.c_str();
	}
	virtual ~VarNotFoundException() throw() {}
private:
	std::string msg;
};

class TypeNotFoundException : public Exception
{
public:
	TypeNotFoundException() : msg("Not found, type does not exist in the container") {}
	TypeNotFoundException(std::string& varname)
	{
		msg = "Variable: \"" + varname + "\" not found, it's type does not exist in the container";
	}
	virtual ~TypeNotFoundException() throw() {}
private:
	std::string msg;
};

//**********************************************************************************************************************
//base class
//**********************************************************************************************************************

class Base
{
public:
    virtual void * get_type() = 0;
    virtual void set(std::string& p_name, void* ptr) = 0;
    virtual void get(std::string& p_name, void* ptr) = 0;
    virtual void getarray(std::vector<std::string> names, void * arr_ptr) = 0;
    virtual void setarray(std::vector<std::string>& names, void * arr_ptr) = 0;
};

//**********************************************************************************************************************
// derived class with template
//**********************************************************************************************************************

template<class T>
class List : public Base
{
public:    
    static void * get_static_type() {return &type;}
    void * get_type() {return &type;}
    void set(std::string& p_name, void * var)
    {
        ptrvec.push_back(*(T*)var);
        names.push_back(p_name);
    }
    void get(std::string& p_name, void * var)
    {
    	T* ptr = (T*) var;
        int n = ptrvec.size() - 1;
        while (n >= 0)
        {
            if (p_name == names[n])
            {
                *ptr = ptrvec[n];
                return;
            }
            --n;
        }
        throw (VarNotFoundException(p_name));
    }
    void getarray(std::vector<std::string> p_names, void * arr_ptr)
    {
        std::vector<T> * array = (std::vector<T>*) arr_ptr;
        int n = ptrvec.size() - 1;
        int m;

        while(n >= 0)
        {
            m = p_names.size() - 1;
            while (m >= 0)
            {
                if (p_names[m] == names[n])
                {
                    array->push_back(ptrvec[n]);
                    p_names.erase(p_names.begin() + m);
                    break;
                }
                --m;
            }
            --n;
        }
        if (p_names.size() != 0) throw (VarNotFoundException());
    }
    void setarray(std::vector<std::string>& p_names, void * arr_ptr)
    {
        std::vector<T> * array = (std::vector<T>*) arr_ptr;
        int n = p_names.size() - 1;
        while (n >= 0)
        {
            names.push_back(p_names[n]);
            ptrvec.push_back(array->at(n));
            --n;
        }
    }
    List()
    {

    }
private:
    std::vector<T> ptrvec;
    std::vector<std::string> names;
    static int type;
};

template<class T>
int List<T>::type;

//**********************************************************************************************************************
// container class
//**********************************************************************************************************************

class Container
{
public:

    ~Container()
    {
        removeall();
    }

    template <class T_get>
    T_get get(std::string& p_name)
    {
    	T_get ret;
        int n = vlb_ptr.size() - 1;
        while (n >= 0)
        {
            if(vlb_ptr[n]->get_type() == List<T_get>::get_static_type())
            {
                vlb_ptr[n]->get(p_name, (void*) &ret);
                return ret;
            }
            --n;
        }
        throw (TypeNotFoundException(p_name));
    }

    template <class T_set>
    void set(std::string p_name, T_set var)
    {
        int n = vlb_ptr.size() - 1;
        while (n >= 0)
        {
            if (vlb_ptr[n]->get_type() == List<T_set>::get_static_type())
            {
                return vlb_ptr[n]->set(p_name, (void*) &var);
            }            
            --n;
        }
        addtype<T_set>();
        return vlb_ptr.back()->set(p_name, (void*) &var);
    }

    template <class T_array>
    std::vector<T_array> getarray(std::vector<std::string>& names)
    {
    	std::vector<T_array> arr;
        int n = vlb_ptr.size() - 1;
        while (n >= 0)
        {
            if (vlb_ptr[n]->get_type() == List<T_array>::get_static_type())
            {
                vlb_ptr[n]->getarray(names, (void*) &arr);
                return arr;
            }
            --n;
        }
        throw (TypeNotFoundException());
    }

    template <class T_addarray>
    void setarray(std::vector<std::string>& names, std::vector<T_addarray>& vars)
    {
        int n = vlb_ptr.size() - 1;
        if (vars.size() != names.size())
        {
        	throw (Exception("Array sizes don't match"));
        	return;
        }
        while (n >= 0)
        {
            if (vlb_ptr[n]->get_type() == List<T_addarray>::get_static_type())
            {
                vlb_ptr[n]->setarray(names, (void*) &vars);
            }
            --n;
        }
        addtype<T_addarray>();
        vlb_ptr.back()->setarray(names, (void*) &vars);
    }

private:

    template <class T_add>
    void addtype()
    {
        Base * vl_ptr;
        vl_ptr = new List<T_add>();
        vlb_ptr.push_back(vl_ptr);
    }

    void removeall()
    {
        int n = vlb_ptr.size() - 1;
        while (n >= 0)
        {
            delete vlb_ptr[n];
            --n;
        }
        vlb_ptr.clear();
    }

    std::vector<Base*> vlb_ptr;

};

}

#endif	/* _VARLIST_H */

