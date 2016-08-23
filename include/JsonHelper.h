#pragma once

inline void operator<<( Json::Value& value, GPSLocation & location )
{
	value.append( location._lat );
	value.append( location._lng );
	value.append( location._height );
}

inline void operator>>( Json::Value& arr, GPSLocation & location )
{
	Json::Value defaultValue = 0.0;
	Json::Value lat = arr.get(Json::Value::UInt(0), defaultValue);
	Json::Value lng = arr.get(Json::Value::UInt(1), defaultValue);
	Json::Value height = arr.get(Json::Value::UInt(2), defaultValue);

	location._lat = lat.asDouble();
	location._lng = lng.asDouble();
	location._height = height.asDouble();
}

template<typename T>
void operator<<( Json::Value& value, Vector<T> & location )
{
	value.append( location.x );
	value.append( location.y );
	value.append( location.z );
}

template<typename T>
void operator>>( Json::Value& arr, Vector<T> & location )
{
	Json::Value defaultValue = 0.0;
	location.x = arr.get(Json::Value::UInt(0), defaultValue).asDouble();
	location.y = arr.get(Json::Value::UInt(1), defaultValue).asDouble();
	location.z = arr.get(Json::Value::UInt(2), defaultValue).asDouble();
}