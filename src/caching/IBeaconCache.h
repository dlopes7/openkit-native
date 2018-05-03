/**
* Copyright 2018 Dynatrace LLC
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

#ifndef _CACHING_IBEACONCACHE_H
#define _CACHING_IBEACONCACHE_H

#include "caching/IObserver.h"
#include "core/UTF8String.h"

#include <cstdint>
#include <memory>
#include <unordered_set>

namespace caching
{
	///
	/// Beacon Cache used to cache the Beacons generated by all sessions, actions, ...
	///
	class IBeaconCache
	{
	public:
		///
		/// Destructor
		///
		virtual ~IBeaconCache() {}

		///
		/// Add an @c IObserver which gets notified after a new event data or action data got inserted.
		/// @param[in] observer
		///
		virtual void addObserver(IObserver* observer) = 0;

		///
		/// Add event data for a given @c beaconID to this cache.
		///
		/// All registered observers are notified, after the event data has been added.
		///
		/// @param[in] beaconID The beacon's ID (aka Session ID) for which to add event data.
		/// @param[in] timestamp The data's timestamp.
		/// @param[in] data serialized event data to add.
		///
		virtual void addEventData(int32_t beaconID, int64_t timestamp, const core::UTF8String& data) = 0;

		///
		/// Add action data for a given @c beaconID to this cache.
		///
		/// @param[in] beaconID The beacon's ID (aka Session ID) for which to add action data.
		/// @param[in] timestamp The data's timestamp.
		/// @param[in] data serialized action data to add.
		///
		virtual void addActionData(int32_t beaconID, int64_t timestamp, const core::UTF8String& data) = 0;

		///
		/// Delete a cache entry for a given @c beaconID.
		/// @param[in] beaconID The beacon's ID (aka Session ID) which to delete.
		///
		virtual void deleteCacheEntry(int32_t beaconID) = 0;

		///
		/// Get the next chunk for sending to the backend.
		///
		/// Note: This method must only be invoked from the beacon sending thread.
		///
		/// @param[in] beaconID The beacon id for which to get the next chunk.
		/// @param[in] chunkPrefix Prefix to append to the beginning of the chunk.
		/// @param[in] maxSize Maximum chunk size. As soon as chunk's size is greater than or equal to maxSize result is returned.
		/// @param[in] delimiter Delimiter between consecutive chunks.
		/// @return the next chunk to send or an empty string, if either the given @c beaconID does not exist or if there is no more data to send.
		///
		virtual const core::UTF8String getNextBeaconChunk(int32_t beaconID, const core::UTF8String& chunkPrefix, int32_t maxSize, const core::UTF8String& delimiter) = 0;

		///
		/// Remove all data that was previously included in chunks.
		///
		/// This method must be called, when data retrieved via @ref getNextBeaconChunk was successfully sent to the backend,
		/// otherwise subsequent calls to @ref getNextBeaconChunk will retrieve the same data again and again.
		///
		/// Note: This method must only be invoked from the beacon sending thread.
		///
		/// @param[in] beaconID The beacon id for which to remove already chunked data.
		///
		virtual void removeChunkedData(int32_t beaconID) = 0;

		///
		/// Reset all data that was previously included in chunks.
		///
		/// Note: This method must only be invoked from the beacon sending thread.
		///
		/// @param[in] beaconID The beacon id for which to remove already chunked data.
		///
		virtual void resetChunkedData(int32_t beaconID) = 0;

		///
		/// Get a Set of currently inserted Beacon ids.
		///
		/// The return value is a snapshot of currently inserted beacon ids.
		/// All changes made after this call are not reflected in the returned Set.
		///
		/// @return Snapshot of all beacon ids in the cache.
		///
		virtual const std::unordered_set<int32_t> getBeaconIDs() = 0;

		///
		/// Evict @ref BeaconCacheRecord by age for a given beacon.
		///
		/// @param[in] beaconID      The beacon's identifier.
		/// @param[in] minTimestamp  The minimum timestamp allowed.
		/// @return Returns the number of evicted cache records.
		///
		virtual uint32_t evictRecordsByAge(int32_t beaconID, int64_t minTimestamp) = 0;

		///
		/// Evict @ref BeaconCacheRecord by number for given beacon.
		///
		/// @param[in] beaconID   The beacon's identifier.
		/// @param[in] numRecords The maximum number of records to evict.
		/// @return Returns the number of evicted cache records.
		///
		virtual uint32_t evictRecordsByNumber(int32_t beaconID, uint32_t numRecords) = 0;

		///
		/// Get number of bytes currently stored in cache.
		///
		/// @return Number of bytes currently stored in cache.
		///
		virtual int64_t getNumBytesInCache() const = 0;

		///
		/// Tests if an cached entry for @c beaconID is empty.
		///
		/// @param[in] beaconID The beacon's identifier.
		/// @return @c true if the cached entry is empty, @c false otherwise.
		///
		virtual bool isEmpty(int32_t beaconID) = 0;
	};
}

#endif
