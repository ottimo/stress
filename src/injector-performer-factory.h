/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * stress
 * Copyright (C) Matteo Rosi 2011 <matteo.rosi@gmail.com>
 * 
stress is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * stress is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _INJECTOR_PERFORMER_FACTORY_H_
#define _INJECTOR_PERFORMER_FACTORY_H_

#include <configurator.h>
#include <abstract-injector-performer.h>
#include <InjectorPerformer.h>

class InjectorPerformerFactory
{
	public:
		~InjectorPerformerFactory();
		static InjectorPerformerFactory* getInstance();
		AbstractInjectorPerformer* getPerformer();
	protected:
		InjectorPerformerFactory();
	private:
		static InjectorPerformerFactory* _instance;
};

#endif // _INJECTOR_PERFORMER_FACTORY_H_
