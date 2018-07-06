#pragma once

#include <algorithm>
#include <atomic>
#include <memory>
#include <vector>

template <class SettingsType>
class SettingsSubject;

template <class SettingsType>
class SettingsObserver
{
public:
    SettingsObserver(SettingsSubject<SettingsType>& subject):
        m_currentSettings(std::make_shared<SettingsType>())
    {
        subject.registerObserver(this);
    }

    void setSettings(const SettingsType& s)
    {
        auto newSettings = std::make_shared<SettingsType>(s);
        updatePool(newSettings);

        std::atomic_store(&m_currentSettings, newSettings);
        m_settingsChanged = true;
    }

protected:
    std::shared_ptr<SettingsType> getSettings() const
    {
        return std::atomic_load(&m_currentSettings);
    }
    std::atomic_bool m_settingsChanged;

private:
    std::shared_ptr<SettingsType> m_currentSettings;

    //This is to avoid calling delete on the render thread
    std::vector<std::shared_ptr<SettingsType>> m_pool;
    void updatePool(std::shared_ptr<SettingsType> obj)
    {
        //All shared ptr will be deleted here
        m_pool.erase(std::remove_if(m_pool.begin(), m_pool.end(), [](auto& obj)
                     {return obj.use_count() <= 1;}),
                     m_pool.end());

        m_pool.push_back(obj);
    }
};


//--------------------------------------------------------//


template <class SettingsType>
class SettingsSubject
{
public:
    void registerObserver(SettingsObserver<SettingsType>* func)
    {
        m_observers.push_back(func);
    }

protected:
    template <typename Type, typename Member>
    void setParameter(Type t, Member m)
    {
        m_settings.*m = t;
        updateObservers();
    }

    SettingsType& getSettings() {return m_settings;}

private:
    void updateObservers()
    {
        std::for_each(m_observers.begin(), m_observers.end(),
                      [&](SettingsObserver<SettingsType>* o) {o->setSettings(m_settings);});
    }

    SettingsType m_settings;
    std::vector<SettingsObserver<SettingsType>*> m_observers;
};




