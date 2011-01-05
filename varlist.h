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

//**********************************************************************************************************************
//base class
//**********************************************************************************************************************

class varListBase
{
public:
    virtual void * get_type(){}
    virtual void * retrieve(std::string p_name){}
    virtual bool set(std::string p_name, void* ptr){return false;}
    virtual bool get(std::string p_name, void* ptr){return false;}
    virtual bool getarray(std::vector<std::string> names, void * arr_ptr) {return false;}
    virtual bool setarray(std::vector<std::string> names, void * arr_ptr) {return false;}
    virtual ~varListBase() {}
};

//**********************************************************************************************************************
// derived class with template
//**********************************************************************************************************************

template<class T>
class varList : public varListBase
{
public:    
    static void * get_static_type() {return &type;}
    void * get_type() {return &type;}
    bool set(std::string p_name, void * var)
    {
        ptrvec.push_back(*(T*)var);
        names.push_back(p_name);
        return true;
    }
    bool get(std::string p_name, void *var)
    {
        int n = ptrvec.size() - 1;
        while (n >= 0)
        {
            if (p_name == names[n])
            {
                *(T*)var = ptrvec[n];
                return true;
            }
            --n;
        }
        return false;
    }
    void * retrieve(std::string p_name)
    {
        int n = ptrvec.size() - 1;
        while (n >= 0)
        {
            if (p_name == names[n])
            {
                return (void*)&ptrvec[n];
            }
            --n;
        }
        return NULL;
    }
    bool getarray(std::vector<std::string> p_names, void * arr_ptr)
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

    }
    bool setarray(std::vector<std::string> p_names, void * arr_ptr)
    {
        std::vector<T> * array = (std::vector<T>*) arr_ptr;
        int n = p_names.size() - 1;
        while (n >= 0)
        {
            names.push_back(p_names[n]);
            ptrvec.push_back(array->at(n));
            --n;
        }
        return true;
    }
    ~varList()
    {

    }
private:
    std::vector<T> ptrvec;
    std::vector<std::string> names;
    static int type;
};

template<class T>
int varList<T>::type;

//**********************************************************************************************************************
// container class
//**********************************************************************************************************************

class varListContainer
{
private:
    std::vector<varListBase*> vlb_ptr;
public:
    template <class T_get>
    bool get(std::string p_name, T_get * var)
    {
        int n = vlb_ptr.size() - 1;
        while (n >= 0)
        {
            if(vlb_ptr[n]->get_type() == varList<T_get>::get_static_type())
            {
                return vlb_ptr[n]->get(p_name, (void*) var);
            }
            --n;
        }
        return false;
    }
    template <class T_set>
    bool set(std::string p_name, T_set var)
    {
        int n = vlb_ptr.size() - 1;
        while (n >= 0)
        {
            
            if (vlb_ptr[n]->get_type() == varList<T_set>::get_static_type())
            {
                return vlb_ptr[n]->set(p_name, (void*)&var);
            }            
            --n;
        }
        addtype<T_set>();
        return vlb_ptr.back()->set(p_name, (void*)&var);
    }
    template <class T_add>
    void addtype()
    {
        varListBase * vl_ptr;
        vl_ptr = new varList<T_add>();
        vlb_ptr.push_back(vl_ptr);
    }
    template <class T_array>
    bool getarray(std::vector<std::string> names, std::vector<T_array> * vars)
    {
        int n = vlb_ptr.size() - 1;
        if (vars->size() != 0) return false;
        while (n >= 0)
        {
            if (vlb_ptr[n]->get_type() == varList<T_array>::get_static_type())
            {
                return vlb_ptr[n]->getarray(names, (void*) vars);
            }
            --n;
        }
    }
    template <class T_addarray>
    bool setarray(std::vector<std::string> names, std::vector<T_addarray> vars)
    {
        int n = vlb_ptr.size() - 1;
        if (vars.size() != names.size()) return false;
        while (n >= 0)
        {
            if (vlb_ptr[n]->get_type() == varList<T_addarray>::get_static_type())
            {
                return vlb_ptr[n]->setarray(names, (void*) &vars);
            }
            --n;
        }
        addtype<T_addarray>();
        return vlb_ptr.back()->setarray(names, (void*) &vars);
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
    ~varListContainer()
    {
        removeall();
    }
    template <class T_remove>
    void removetype()
    {
        int n = vlb_ptr.size() - 1;
        while (n >= 0)
        {
            if (vlb_ptr[n]->get_type() == varList<T_remove>::get_static_type())
            {
                delete vlb_ptr[n];
                vlb_ptr.erase(vlb_ptr.begin() + n);
                return;
            }
            --n;
        }
    }
};

#endif	/* _VARLIST_H */

