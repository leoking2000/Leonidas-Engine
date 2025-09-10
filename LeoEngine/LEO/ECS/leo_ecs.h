#pragma once
#include <array>
#include <bitset>
#include <vector>
#include <ranges>

#include "LEO/Log/Log.h"

namespace LEO
{
	using entity_id = u64;

	template<typename T, u64 SIZE>
	class ComponentArray
	{
	public:
		void AddComponent(entity_id id, T component) 
		{ 
			LEOASSERTF(id < m_data.size(), "Invalid ID: {}, size of ComponentArray is {}", id, m_data.size());
			LEOASSERTF(!m_exist[id], "Entity with ID: {} already has the component", id);

			m_toAdd.emplace_back(id, std::move(component)); 
		}

		void RemoveComponent(entity_id id)           
		{ 
			LEOASSERTF(id < m_data.size(), "Invalid ID: {}, size of ComponentArray is {}", id, m_data.size());
			LEOASSERTF(m_exist[id], "Entity with ID: {} does not have the component", id);

			m_toRemove.emplace_back(id); 
		}
		
		bool HasComponent(entity_id id) const        
		{ 
			LEOASSERTF(id < m_data.size(), "Invalid ID: {}, size of ComponentArray is {}", id, m_data.size());
			return m_exist[id]; 
		}

		T*   GetComponent(entity_id id)              
		{ 
			LEOASSERTF(id < m_data.size(), "Invalid ID: {}, size of ComponentArray is {}", id, m_data.size());
			return m_exist[id] ? &m_data[id] : nullptr; 
		}

		u64 NumOfComponents() const { return m_count; }
	public:
		void ApplyPending() {
			// Remove pending components
			for (entity_id id : m_toRemove) {
				if (m_exist[id]) {
					m_exist[id] = false;
					std::destroy_at(&m_data[id]);
					m_count--;
				}
			}
			m_toRemove.clear();

			// Add pending components
			for (auto& [id, comp] : m_toAdd) {
				if (!m_exist[id]) {
					m_exist[id] = true;
					m_count++;
					
				}
				else {
					std::destroy_at(&m_data[id]);
				}
				m_data[id] = std::move(comp);
			}
			m_toAdd.clear();
		}
	public:
		struct Iterator {
			u64 index;
			ComponentArray* parent;

			bool operator!=(const Iterator& other) const { return index != other.index; }

			// Prefix ++ : increment and return reference
			Iterator& operator++() {
				do { ++index; } while (index < SIZE && !parent->m_exist[index]);
				return *this;
			}

			// Postfix ++ : return copy before increment
			Iterator operator++(int) {
				Iterator temp = *this;
				++(*this); // use prefix
				return temp;
			}

			struct Item {
				entity_id id;
				T& comp;
			};
			Item operator*() const {
				return { index, parent->m_data[index] };
			}

			// next helper: advance by n steps
			Iterator next(size_t n = 1) const {
				Iterator it = *this;
				for (size_t i = 0; i < n; ++i) ++it;
				return it;
			}
		};

		Iterator begin() {
			u64 start = 0;
			while (start < SIZE && !m_exist[start]) ++start;
			return Iterator{ start, this };
		}

		Iterator end() { return Iterator{ SIZE, this }; }
	private:
		std::array<T, SIZE> m_data = {};
		std::bitset<SIZE> m_exist  = {};

		std::vector<std::pair<entity_id, T>> m_toAdd;
		std::vector<entity_id> m_toRemove;

		u64 m_count = 0;
	};

}

