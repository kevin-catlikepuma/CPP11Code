/**
 * @file   INotifySink.h
 * @author ZENG Jie
 * @date   Mon Jul 18 16:28:31 2005
 * 
 * @brief  INotifySink�ӿڶ���
 * 
 * 
 * 
 * @version $Id$
 * 
 * Change Log: 
 * CVS $Log$
 */


#ifndef _INOTIFYSINK_H
#define _INOTIFYSINK_H

#include <string>

/**
 * �������
 * 
 */
class BizObjectProxy
{
public:
    /// ҵ��ģ������
    std::wstring model;
    /// ҵ�����ID
    std::wstring id;
    /// �û��Զ����ֶ�
    std::wstring user;

    /** 
     * 
     * 
     * @param theModel ҵ��ģ������
     * @param theId ҵ�����ID
     * @param theUser �û��Զ����ֶ�
     */
    BizObjectProxy(std::wstring theModel = L"",
                   std::wstring theId = L"",
                   std::wstring theUser = L"")
        : model(theModel)
        , id(theId)
        , user(theUser)
        {}
};

/**
 * ֪ͨ���սӿ�
 * 
 */
class INotifySink
{
public:
    /** 
     * ����һ������ʱ������
     * 
     * @param object �������
     */
    virtual void onAdd(BizObjectProxy object) = 0;
    
    /** 
     * ����һ������ʱ������
     * 
     * @param object �������
     */
    virtual void onRemove(BizObjectProxy object) = 0;

    /**
     * ����ı�ʱ������
     *
     * @param object ������� 
     */

    virtual void onChange(BizObjectProxy object) {}

    /**
     * ��������ʱ������
     * 
     * ����ͻ���ʹ��onAdd��onRemoveά���б���ʱӦ����б����½���֪ͨ��
     */

    virtual void onReset() {}
};


#endif /* _INOTIFYSINK_H */
