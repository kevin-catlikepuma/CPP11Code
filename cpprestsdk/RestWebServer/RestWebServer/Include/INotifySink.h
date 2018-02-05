/**
 * @file   INotifySink.h
 * @author ZENG Jie
 * @date   Mon Jul 18 16:28:31 2005
 * 
 * @brief  INotifySink接口定义
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
 * 对象代理
 * 
 */
class BizObjectProxy
{
public:
    /// 业务模型名称
    std::wstring model;
    /// 业务对象ID
    std::wstring id;
    /// 用户自定义字段
    std::wstring user;

    /** 
     * 
     * 
     * @param theModel 业务模型名称
     * @param theId 业务对象ID
     * @param theUser 用户自定义字段
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
 * 通知接收接口
 * 
 */
class INotifySink
{
public:
    /** 
     * 增加一个对象时被调用
     * 
     * @param object 对象代理
     */
    virtual void onAdd(BizObjectProxy object) = 0;
    
    /** 
     * 减少一个对象时被调用
     * 
     * @param object 对象代理
     */
    virtual void onRemove(BizObjectProxy object) = 0;

    /**
     * 对象改变时被调用
     *
     * @param object 对象代理 
     */

    virtual void onChange(BizObjectProxy object) {}

    /**
     * 连接重置时被调用
     * 
     * 如果客户端使用onAdd和onRemove维护列表，此时应清空列表重新接收通知。
     */

    virtual void onReset() {}
};


#endif /* _INOTIFYSINK_H */
