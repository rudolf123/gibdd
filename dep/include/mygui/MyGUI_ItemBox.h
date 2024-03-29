/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_ITEM_BOX_H__
#define __MYGUI_ITEM_BOX_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_DDContainer.h"
#include "MyGUI_IBItemInfo.h"
#include "MyGUI_Any.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_ScrollViewBase.h"

namespace MyGUI
{

	typedef delegates::CDelegate2<ItemBox*, Widget*> EventHandle_ItemBoxPtrWidgetPtr;
	typedef delegates::CDelegate3<ItemBox*, IntCoord&, bool> EventHandle_ItemBoxPtrIntCoordRefBool;
	typedef delegates::CDelegate3<ItemBox*, Widget*, const IBDrawItemInfo &> EventHandle_ItemBoxPtrWidgetPtrCIBCellDrawInfoRef;
	typedef delegates::CDelegate2<ItemBox*, size_t> EventHandle_ItemBoxPtrSizeT;
	typedef delegates::CDelegate2<ItemBox*, const IBNotifyItemData &> EventHandle_ItemBoxPtrCIBNotifyCellDataRef;

	class MYGUI_EXPORT ItemBox :
		public DDContainer,
		protected ScrollViewBase
	{
		MYGUI_RTTI_DERIVED( ItemBox )

	public:
		ItemBox();

		//------------------------------------------------------------------------------//
		// манипуляции айтемами

		//! Get number of items
		size_t getItemCount() const { return mItemsInfo.size(); }

		//! Insert an item into a array at a specified position
		void insertItemAt(size_t _index, Any _data = Any::Null);

		//! Add an item to the end of a array
		void addItem(Any _data = Any::Null) { insertItemAt(ITEM_NONE, _data); }

		//! Remove item at a specified position
		void removeItemAt(size_t _index);

		//! Remove all items
		void removeAllItems();

		//! Redraw at a specified position
		void redrawItemAt(size_t _index);

		//! Redraw all items
		void redrawAllItems();


		//------------------------------------------------------------------------------//
		// манипуляции выделениями

		//! Get index of selected item (ITEM_NONE if none selected)
		size_t getIndexSelected() { return mIndexSelect; }

		//! Select specified _index
		void setIndexSelected(size_t _index);

		//! Clear item selection
		void clearIndexSelected() { setIndexSelected(ITEM_NONE); }


		//------------------------------------------------------------------------------//
		// манипуляции данными

		//! Replace an item data at a specified position
		void setItemDataAt(size_t _index, Any _data);

		//! Clear an item data at a specified position
		void clearItemDataAt(size_t _index) { setItemDataAt(_index, Any::Null); }

		//! Get item data from specified position
		template <typename ValueType>
		ValueType * getItemDataAt(size_t _index, bool _throw = true)
		{
			MYGUI_ASSERT_RANGE(_index, mItemsInfo.size(), "ItemBox::getItemDataAt");
			return mItemsInfo[_index].data.castType<ValueType>(_throw);
		}


		/** Set vertical alignment grid mode */
		void setItemBoxAlignVert(bool _value);
		/** Get vertical alignment grid mode flag */
		bool getItemBoxAlignVert() { return mAlignVert; }

		/** Get item index by item Widget pointer */
		size_t getIndexByWidget(Widget* _widget);

		/** Get widget created for drop */
		Widget* getWidgetDrag() { return mItemDrag; }

		/** Get item Widget pointer by item index if it is visible
			@note returned widget can be deleted, so this pointer
			is valid only at time when you got it and can be invalid
			next frame
		*/
		Widget* getWidgetByIndex(size_t _index);

		/** Interrupt drag as if widget was dropped into empty space */
		void resetDrag();

		//! @copydoc Widget::setPosition(const IntPoint& _value)
		virtual void setPosition(const IntPoint& _value);
		//! @copydoc Widget::setSize(const IntSize& _value)
		virtual void setSize(const IntSize& _value);
		//! @copydoc Widget::setCoord(const IntCoord& _value)
		virtual void setCoord(const IntCoord& _value);

		/** @copydoc Widget::setPosition(int _left, int _top) */
		void setPosition(int _left, int _top) { setPosition(IntPoint(_left, _top)); }
		/** @copydoc Widget::setSize(int _width, int _height) */
		void setSize(int _width, int _height) { setSize(IntSize(_width, _height)); }
		/** @copydoc Widget::setCoord(int _left, int _top, int _width, int _height) */
		void setCoord(int _left, int _top, int _width, int _height) { setCoord(IntCoord(_left, _top, _width, _height)); }

	/*event:*/
		/** Event : request for creating new item
			signature : void method(MyGUI::ItemBox* _sender, MyGUI::Widget* _item)
			@param _sender widget that called this event
			@param _item widget item pointer
		*/
		EventHandle_ItemBoxPtrWidgetPtr requestCreateWidgetItem;

		/** Event : request for item coordinate
			signature : void method(MyGUI::ItemBox* _sender, MyGUI::IntCoord& _coord, bool _drag)
			@param _sender widget that called this event
			@param _coord write heer item coordinate
			@param _drag is this item dragging
		*/
		EventHandle_ItemBoxPtrIntCoordRefBool requestCoordItem;

		/** Event : request for item redraw
			signature : void method(MyGUI::ItemBox* _sender, MyGUI::Widget* _item, const MyGUI::IBDrawItemInfo& _info)
			@param _sender widget that called this event
			@param _item widget item pointer
			@param _info item info
		*/
		EventHandle_ItemBoxPtrWidgetPtrCIBCellDrawInfoRef requestDrawItem;

		/** Event : doubleclick or enter pressed on item
			signature : void method(MyGUI::ItemBox* _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ItemBoxPtrSizeT eventSelectItemAccept;

		/** Event : position of selected item was changed
			signature : void method(MyGUI::ItemBox* _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ItemBoxPtrSizeT eventChangeItemPosition;

		/** Event : click on item
			signature : void method(MyGUI::ItemBox* _sender, size_t _index)
			@param _sender widget that called this event
			@param _index item index
		*/
		EventHandle_ItemBoxPtrSizeT eventMouseItemActivate;

		/** Event : notify about event in item widget
			signature : void method(MyGUI::ItemBox* _sender, const MyGUI::IBNotifyItemData& _info)
			@param _sender widget that called this event
			@param _info info about item notify
		*/
		EventHandle_ItemBoxPtrCIBNotifyCellDataRef eventNotifyItem;


	/*internal:*/
		virtual void _initialise(WidgetStyle _style, const IntCoord& _coord, Align _align, ResourceSkin* _info, Widget* _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name);

	/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : void Widget::setCoord(const IntCoord& _coord)")
		void setPosition(const IntCoord& _coord) { setCoord(_coord); }
		MYGUI_OBSOLETE("use : void Widget::setCoord(int _left, int _top, int _width, int _height)")
		void setPosition(int _left, int _top, int _width, int _height) { setCoord(_left, _top, _width, _height); }

		MYGUI_OBSOLETE("use : size_t ItemBox::getItemIndexSelected()")
		size_t getItemIndexSelected() { return getIndexSelected(); }
		MYGUI_OBSOLETE("use : void ItemBox::setIndexSelected(size_t _index)")
		void setItemSelectedAt(size_t _index) { setIndexSelected(_index); }
		MYGUI_OBSOLETE("use : void ItemBox::clearIndexSelected()")
		void clearItemSelected() { clearIndexSelected(); }

		MYGUI_OBSOLETE("use : void ItemBox::insertItemAt(size_t _index, Any _data)")
		void insertItem(size_t _index, Any _data = Any::Null) { insertItemAt(_index, _data); }
		MYGUI_OBSOLETE("use : void ItemBox::setItemDataAt(size_t _index, Any _data)")
		void setItemData(size_t _index, Any _data) { setItemDataAt(_index, _data); }
		MYGUI_OBSOLETE("use : void ItemBox::removeItemAt(size_t _index)")
		void deleteItem(size_t _index) { removeItemAt(_index); }
		MYGUI_OBSOLETE("use : void ItemBox::removeAllItems()")
		void deleteAllItems() { removeAllItems(); }
		MYGUI_OBSOLETE("use : size_t ItemBox::getIndexSelected()")
		size_t getItemSelect() { return getIndexSelected(); }
		MYGUI_OBSOLETE("use : void ItemBox::clearIndexSelected()")
		void resetItemSelect() { clearIndexSelected(); }
		MYGUI_OBSOLETE("use : void ItemBox::setIndexSelected(size_t _index)")
		void setItemSelect(size_t _index) { setIndexSelected(_index); }

		MYGUI_OBSOLETE("use : Widget* ItemBox::getWidgetDrag()")
		Widget* getWidgetDrop() { return getWidgetDrag(); }
		MYGUI_OBSOLETE("use : void ItemBox::resetDrag()")
		void resetDrop() { resetDrag(); }

#endif // MYGUI_DONT_USE_OBSOLETE


	protected:
		virtual ~ItemBox();

		struct ItemDataInfo
		{
			ItemDataInfo(Any _data) :
				data(_data) { }
			Any data;
		};
		typedef std::vector<ItemDataInfo> VectorItemInfo;

		void baseChangeWidgetSkin(ResourceSkin* _info);

		virtual void onMouseButtonPressed(int _left, int _top, MouseButton _id);
		virtual void onMouseButtonReleased(int _left, int _top, MouseButton _id);
		virtual void onMouseDrag(int _left, int _top);

		virtual void onMouseWheel(int _rel);
		virtual void onKeyLostFocus(Widget* _new);
		virtual void onKeySetFocus(Widget* _old);

		void notifyKeyButtonPressed(Widget* _sender, KeyCode _key, Char _char);
		void notifyKeyButtonReleased(Widget* _sender, KeyCode _key);
		void notifyScrollChangePosition(VScroll* _sender, size_t _index);
		void notifyMouseWheel(Widget* _sender, int _rel);
		void notifyRootMouseChangeFocus(Widget* _sender, bool _focus);
		void notifyMouseButtonDoubleClick(Widget* _sender);
		void _requestGetContainer(Widget* _sender, Widget*& _container, size_t& _index);
		void notifyMouseDrag(Widget* _sender, int _left, int _top);
		void notifyMouseButtonPressed(Widget* _sender, int _left, int _top, MouseButton _id);
		void notifyMouseButtonReleased(Widget* _sender, int _left, int _top, MouseButton _id);


		virtual void removeDropItems();
		virtual void updateDropItems();
		virtual void updateDropItemsState(const DDWidgetState& _state);

		// Обновляет данные о айтемах, при изменении размеров
		void updateMetrics();

		// просто обновляет все виджеты что видны
		void _updateAllVisible(bool _redraw);

		void updateFromResize();

		// возвращает следующий айтем, если нет его, то создается
		// запросы только последовательно
		Widget* getItemWidget(size_t _index);

		void _setContainerItemInfo(size_t _index, bool _set, bool _accept);

		// сбрасываем старую подсветку
		void resetCurrentActiveItem();
		// ищет и устанавливает подсвеченный айтем
		void findCurrentActiveItem();

		// запрашиваем у конейтера айтем по позиции мыши
		virtual size_t _getContainerIndex(const IntPoint& _point);

		// сбрасывает зависимости, при любом колличественном изменении
		virtual void _resetContainer(bool _update);

	private:
		void initialiseWidgetSkin(ResourceSkin* _info);
		void shutdownWidgetSkin();

		size_t calcIndexByWidget(Widget* _widget);

		void requestItemSize();

		virtual IntSize getContentSize();
		virtual IntPoint getContentPosition();
		virtual IntSize getViewSize() const;
		virtual void eraseContent();
		virtual size_t getHScrollPage();
		virtual size_t getVScrollPage();
		virtual Align getContentAlign();
		virtual void setContentPosition(const IntPoint& _point);

		IntRect _getClientAbsoluteRect();
		Widget* _getClientWidget();
		const Widget* _getClientWidget() const;

	private:
		// наши дети в строках
		VectorWidgetPtr mVectorItems;

		// размер одного айтема
		IntSize mSizeItem;

		// размерность скролла в пикселях
		IntSize mContentSize;
		// позиция скролла п пикселях
		IntPoint mContentPosition;

		// колличество айтемов в одной строке
		int mCountItemInLine;
		// колличество линий
		int mCountLines;

		// самая верхняя строка
		int mFirstVisibleIndex;
		// текущее смещение верхнего элемента в пикселях
		// сколько его пикселей не видно сверху
		int mFirstOffsetIndex;

		// текущий выделенный элемент или ITEM_NONE
		size_t mIndexSelect;
		// подсвеченный элемент или ITEM_NONE
		size_t mIndexActive;
		// индекс со свойством приема или ITEM_NONE
		size_t mIndexAccept;
		// индекс со свойством отказа или ITEM_NONE
		size_t mIndexRefuse;

		// имеем ли мы фокус ввода
		bool mIsFocus;

		// структура данных об айтеме
		VectorItemInfo mItemsInfo;

		Widget* mItemDrag;
		IntPoint mPointDragOffset;

		bool mAlignVert;

		std::string mDragLayer;

	};

} // namespace MyGUI

#endif // __MYGUI_ITEM_BOX_H__
