# geode-bindings-to-ida-script

Simple program that convert geode sdk bindings (GeometryDash.bro) to script for IDA that adding names for functions.

So u can reverse game with already explored by geode modding community game functions names.
## Example
- Input:
```cpp
class AccountHelpLayer : GJDropDownLayer, GJAccountDelegate, FLAlertLayerProtocol {
	// virtual ~AccountHelpLayer();

	static AccountHelpLayer* create() = win 0x58a10;

	TodoReturn doUnlink();
	TodoReturn exitLayer();
	void onAccountManagement(cocos2d::CCObject* sender);
	void onReLogin(cocos2d::CCObject* sender);
	void onUnlink(cocos2d::CCObject* sender);
	TodoReturn updatePage();
	TodoReturn verifyUnlink();

	virtual void customSetup() = win 0x58b90;
	virtual void layerHidden() = win 0x594f0;
	virtual TodoReturn accountStatusChanged() = win 0x59220;
	virtual void FLAlert_Clicked(FLAlertLayer*, bool) = win 0x59480;
}
class FLAlertLayer : cocos2d::CCLayerColor {
	~FLAlertLayer() {
		if (m_forcePrioRegistered) {
			cocos2d::CCTouchDispatcher::get()->unregisterForcePrio(this);
		}
	}
	FLAlertLayer() {
		m_buttonMenu = nullptr;
		m_controlConnected = -1;
		m_mainLayer = nullptr;
		m_ZOrder = 0;
		m_noElasticity = false;
		m_reverseKeyBack = false;
		m_scene = nullptr;
		m_alertProtocol = nullptr;
		m_scrollingLayer = nullptr;
		m_button2 = nullptr;
		m_button1 = nullptr;
		m_joystickConnected = -1;
		m_containsBorder = 0;
		m_forcePrioRegistered = false;
	}

	static FLAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) = win 0x30da0;
	static FLAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width) = win 0x30cf0;
	static FLAlertLayer* create(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2) = win 0x30c40;

	static FLAlertLayer* create(char const* title, const gd::string& desc, char const* btn) {
		return FLAlertLayer::create(nullptr, title, desc, btn, nullptr, 300.0);
	}

	TodoReturn incrementForcePrio();
	bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float, float) = win 0x30f60;
	bool init(int) = win 0x30ea0;
	void onBtn1(cocos2d::CCObject* sender) = win 0x31ab0;
	void onBtn2(cocos2d::CCObject* sender) = win 0x31af0;

	// virtual void onEnter(); // only exists on armv7...
	virtual bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) = win 0x31b30;
	virtual void ccTouchMoved(cocos2d::CCTouch*, cocos2d::CCEvent*) = win 0x31c80;
	virtual void ccTouchEnded(cocos2d::CCTouch*, cocos2d::CCEvent*) = win 0x31bc0;
	virtual void ccTouchCancelled(cocos2d::CCTouch*, cocos2d::CCEvent*) = win 0x31c30;
	virtual void registerWithTouchDispatcher() = win 0x31e60;
	virtual void keyBackClicked() = win 0x31a10;
	virtual void keyDown(cocos2d::enumKeyCodes) = win 0x319a0;
	virtual void show() = win 0x31cd0;

	cocos2d::CCMenu* m_buttonMenu;
	FLAlertLayerProtocol* m_alertProtocol;
	cocos2d::CCNode* m_scene;
	bool m_reverseKeyBack;
	cocos2d::ccColor3B m_color;
	cocos2d::CCLayer* m_mainLayer;
	int m_ZOrder;
	bool m_noElasticity;
	cocos2d::ccColor3B m_color2;
	ButtonSprite* m_button1;
	ButtonSprite* m_button2;
	ScrollingLayer* m_scrollingLayer;
	int m_controlConnected;
	bool m_containsBorder;
	bool m_noAction;
	int m_joystickConnected;
	bool m_forcePrioRegistered;
}
```
- Output:
```py
static main()
{
   auto base = get_imagebase();
   set_name(base + 0x58a10, "AccountHelpLayer_create");
   set_name(base + 0x58b90, "AccountHelpLayer_customSetup");
   set_name(base + 0x594f0, "AccountHelpLayer_layerHidden");
   set_name(base + 0x59220, "AccountHelpLayer_accountStatusChanged");
   set_name(base + 0x59480, "AccountHelpLayer_FLAlert_Clicked");
   set_name(base + 0x30da0, "FLAlertLayer_create");
   set_name(base + 0x30cf0, "FLAlertLayer_create1");
   set_name(base + 0x30c40, "FLAlertLayer_create2");
   set_name(base + 0x30f60, "FLAlertLayer_init");
   set_name(base + 0x30ea0, "FLAlertLayer_init1");
   set_name(base + 0x31ab0, "FLAlertLayer_onBtn1");
   set_name(base + 0x31af0, "FLAlertLayer_onBtn2");
   set_name(base + 0x31b30, "FLAlertLayer_ccTouchBegan");
   set_name(base + 0x31c80, "FLAlertLayer_ccTouchMoved");
   set_name(base + 0x31bc0, "FLAlertLayer_ccTouchEnded");
   set_name(base + 0x31c30, "FLAlertLayer_ccTouchCancelled");
   set_name(base + 0x31e60, "FLAlertLayer_registerWithTouchDispatcher");
   set_name(base + 0x31a10, "FLAlertLayer_keyBackClicked");
   set_name(base + 0x319a0, "FLAlertLayer_keyDown");
   set_name(base + 0x31cd0, "FLAlertLayer_show");
}
