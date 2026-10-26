/*
 * Copyright (C) 2026 Clownacy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef BACKUP_STACK_H
#define BACKUP_STACK_H

#include <stack>
#include <utility>

template<typename T>
class BackupStack : public std::stack<T>
{
public:
	template<typename... Args>
	BackupStack(Args &&...args)
	{
		this->emplace(std::forward<Args>(args)...);
	}

	T& operator*()
	{
		return this->top();
	}

	const T& operator*() const
	{
		return this->top();
	}

	T* operator->()
	{
		return &this->top();
	}

	const T* operator->() const
	{
		return &this->top();
	}
};


#endif /* BACKUP_STACK_H */
