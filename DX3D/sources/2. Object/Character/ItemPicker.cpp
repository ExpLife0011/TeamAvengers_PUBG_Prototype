#include "stdafx.h"
#include "ItemPicker.h"
#include "Item.h"

ItemPicker::ItemPicker(PlayerAni& owner)
    : BaseObject()
    , m_pOwner(&owner)
    , m_pBoxCollider(nullptr)
    , m_pCollisionListener(nullptr)
{
}

ItemPicker::~ItemPicker()
{
}

void ItemPicker::Init(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
    m_pCollisionListener = SetComponent<ItemPickerCollisionListener>();
    m_pBoxCollider = SetComponent<BoxCollider>();
    m_pBoxCollider->Init(D3DXVECTOR3(-3.0f, -1.0f, -3.0f), D3DXVECTOR3(5.0f, 3.0f, 6.0f));
    m_pBoxCollider->SetTag(CollisionTag::kItemPicker);
}

void ItemPicker::Update(vector<Item*>& OutItems, const D3DXMATRIX& transform)
{
    m_pBoxCollider->Update(transform);

    vector<ColliderBase*> collideds;
    g_pCollisionManager->GetCollideds(collideds, *m_pBoxCollider, CollisionTag::kItem);

    OutItems.resize(0);
    
    for (auto c : collideds)
    {
        if (!c) continue;

        BaseObject* o = c->GetOwner();
        if (!o) continue;

        OutItems.emplace_back(static_cast<Item*>(o));
    }
}

ItemPicker* ItemPicker::Create(PlayerAni& owner, const D3DXVECTOR3& pos, const D3DXVECTOR3& rot)
{
    ItemPicker* ret = new ItemPicker(owner);
    ret->Init(pos, rot);
    return ret;
}

ItemPickerCollisionListener::ItemPickerCollisionListener(BaseObject& owner)
    : ICollisionListener(owner)
{
}

void ItemPickerCollisionListener::OnCollisionEnter(const ColliderBase& other)
{
}

void ItemPickerCollisionListener::OnCollisionExit(const ColliderBase& other)
{
}

void ItemPickerCollisionListener::OnCollisionStay(const ColliderBase& other)
{
}
