#include "LFUCache.h"

void KeyList::init(int fq)
{
	freq=fq;
	head=last=newElement<Node<Key>>();
	head->setNext(NULL);
}

void KeyList::destory(){
	while(head){
		key_node pre=head;
		head=head->getNext();
		deleteElement(pre);
	}
}

int KeyList::getFreq(){
	return freq;
}

//添加到头节点
void KeyList::add(key_node &node){
	if(head->getNext()){  //不是第一个节点
		head->getNext()->setPre(node);
	}
	else last=node;  //添加第一个节点
	node->setNext(head->getNext());
	node->setPre(head);
	head->setNext(node);
}

void KeyList::del(key_node &node){
	node->getPre()->setNext(node->getNext());
    if(node->getNext())
        node->getNext()->setPre(node->getPre());
    else
        last=node->getPre();
}

bool KeyList::isEmpty(){
	return head==last;
}

key_node KeyList::getLast(){
	return last;
}




//LFUcache函数定义
void LFUCache::init(){
	capacity = 10;
	head=newElement<Node<KeyList>>();
	head->getValue().init(0);  //keylist进行初始化
	head->setNext(NULL);
}

LFUCache::~LFUCache(){
	while(head){
		freq_node pre=head;
		head=head->getNext();
		pre->getValue().destory();
		deleteElement(pre);
	}
}

//将nowk在fmap中更新
void LFUCache::addfreq(key_node &nowk,freq_node &nowf){
	freq_node nxt;
	//如果没有下一个freq的fnode，就新创建一个freq + 1的fnode，并放在nowf和nowf->next中间
	if(!nowf->getNext()|| nowf->getNext()->getValue().getFreq() != nowf->getValue().getFreq() + 1){
		nxt=newElement<Node<KeyList>>();
		nxt->getValue().init(nowf->getValue().getFreq()+1);
		if(nowf->getNext())
			nowf->getNext()->setPre(nxt);
		nxt->setNext(nowf->getNext());
		nowf->setNext(nxt);
		nxt->setPre(nowf);
	}
	//如果有下一个fnode且频率为原频率+1
	else nxt=nowf->getNext();
	fmap[nowk->getValue().key]=nxt;
	//移动keynode
	if(nowf != head){
		nowf->getValue().del(nowk);
	}
	nxt->getValue().add(nowk);
	//如果删除完nowf为空则删除nowf
    if(nowf != head && nowf->getValue().isEmpty())
		del(nowf);
}

bool LFUCache::get(string &key,string &v){
	if(!capacity) return false;
	lockGuard lock(mutex);
	if(fmap.find(key)!=fmap.end()){
		//命中
		key_node nowk = kmap[key];
		freq_node nowf = fmap[key];
		v += nowk->getValue().value;
		addfreq(nowk,nowf);
		return true;
	}
	return false;
}

// string LFUCache::get(string &key){
// 	if(!capacity) return "";
// 	lockGuard lock(mutex);
// 	if(fmap.find(key)!=fmap.end()){
// 		key_node nowk = kmap[key];
// 		freq_node nowf = fmap[key];
// 		addfreq(nowk,nowf);
// 		return nowk->getValue().value;
// 	}
// 	return "";
// }

void LFUCache::set(string &key,string &v){
	if(!capacity) return;
	lockGuard lock(mutex);
	//如果cap已满，找到freq最低的链表的last, 并删除last
	if(kmap.size() == capacity){
		freq_node headnxt=head->getNext();
		key_node last=headnxt->getValue().getLast();
		headnxt->getValue().del(last);
		kmap.erase(last->getValue().key);
		fmap.erase(last->getValue().key);
		deleteElement(last);
		if(headnxt->getValue().isEmpty())
			del(headnxt);
	}
	//创建新的keynode
	key_node nowk=newElement<Node<Key>>();
	nowk->getValue().key=key;
	nowk->getValue().value=v;
	addfreq(nowk,head);
	kmap[key]=nowk;
}

void LFUCache::del(freq_node &node){
	node->getPre()->setNext(node->getNext());
	if(node->getNext())
    	node->getNext()->setPre(node->getPre());
    node->getValue().destory();
	deleteElement(node);
}

LFUCache& getCache(){
	static LFUCache cache;
	return cache;
}
