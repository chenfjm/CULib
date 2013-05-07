/**  
*  @file	 CUDelegate.h   
*  @brief  
*  @author        
*  @date  
*  @version    
*/
#ifndef __CUDELEGATE_H__
#define __CUDELEGATE_H__

#pragma once
namespace CULib{

/** 委托模式中的虚基类 */ 
class CULIB_API CDelegateBase	 
{
public:
	/** 构造函数，pObject为委托者的对象， pFn为执行方法 */
	CDelegateBase(void* pObject, void* pFn);
	CDelegateBase(const CDelegateBase& rhs);

	virtual ~CDelegateBase();

	/** 比较两个委托者, true 一致 false不同 */
	bool Equals(const CDelegateBase& rhs) const;

	/** 响应委托进行处理 event为响应的事件 */
	bool operator() (void* param);

	/** 拷贝一个委托者 */
	virtual CDelegateBase* Copy() = 0;

protected:
	/** 得到执行方法 */
	void* GetFn();

	/** 得到委托者对象 */
	void* GetObject();

	/** 调用执行方法 */
	virtual bool Invoke(void* param) = 0;

private:
	void* m_pObject;
	void* m_pFn;
};

/** 静态委托，委托者为空，但有执行过程 */
class CDelegateStatic: public CDelegateBase
{
	typedef bool (*Fn)(void*);
public:
	CDelegateStatic(Fn pFn) : CDelegateBase(NULL, pFn) { } 
	CDelegateStatic(const CDelegateStatic& rhs) : CDelegateBase(rhs) { } 
	virtual CDelegateBase* Copy() { return new CDelegateStatic(*this); }

protected:
	virtual bool Invoke(void* param)
	{
		Fn pFn = (Fn)GetFn();
		return (*pFn)(param); 
	}
};

template <class O, class T>
class CDelegate : public CDelegateBase
{
	typedef bool (T::* Fn)(void*);
public:
	CDelegate(O* pObj, Fn pFn) : CDelegateBase(pObj, &pFn), m_pFn(pFn) { }
	CDelegate(const CDelegate& rhs) : CDelegateBase(rhs) { m_pFn = rhs.m_pFn; } 
	virtual CDelegateBase* Copy() { return new CDelegate(*this); }

protected:
	virtual bool Invoke(void* param)
	{
		O* pObject = (O*) GetObject();
		return (pObject->*m_pFn)(param); 
	}  

private:
	Fn m_pFn;
};

/** 产生委托 */
template <class O, class T>
CDelegate<O, T> MakeDelegate(O* pObject, bool (T::* pFn)(void*))
{
	return CDelegate<O, T>(pObject, pFn);
}
inline CDelegateStatic MakeDelegate(bool (*pFn)(void*))
{
	return CDelegateStatic(pFn); 
}

/** 事件源 */
class CULIB_API CEventSource
{
	typedef bool (*FnType)(void*);
public:
	~CEventSource();
	operator bool();

	/** 加入一个委托者 */
	void operator+= (CDelegateBase& d);
	void operator+= (FnType pFn);

	/** 删除一个委托者 */
	void operator-= (CDelegateBase& d);
	void operator-= (FnType pFn);

	/** 响应事件进行处理 */
	bool operator() (void* param);

protected:
	CStdPtrArray m_aDelegates;
};

}
#endif