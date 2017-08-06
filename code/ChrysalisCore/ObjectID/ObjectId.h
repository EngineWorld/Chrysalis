#pragma once


namespace Chrysalis
{
/** An object identifier used where a globally unique identifier is required. Id's need to be generated by a
specialised factory class that is capable of encapsulating all the rules for making a useful Id with minimal
or no chance of clashes. This needs to be OS agnostic are memory sensible.

The ID is tightly packed into a single 64 bit unsigned integer value. With 0 representing the highest order
bit (will this work on big endian machines?), the bits are laid out as follows. Do not rely on this for a sort
order, since clocks may not be correctly synced between systems. Don't be tempted to directly twiddle with
the bits, use the accessors provided instead.

0-31	:	Number of seconds since Unix epoch.
32-49	:	An instance ID that is unique for every running instance of this code. These need to be unique
and should be carefully assigned on a as-needed basis.
50-63	:	A random component. Each second a new starting point is generated, and it increments with each
Id generated during that second. This forces a hard limit on the number of Ids that can be generated
in a single second. Currently this limit is 16,384. /
*/

typedef uint64_t ObjectId;


/** A factory class needed to properly construct valid ObjectIds. The class is dependant on
having a valid instanceId prior to instantiation.

It is recommended that you create an instance of this factory for every entity class where you think
you will need to create more the hard limit of ObjectId's / second. Currently, this limit is 16,384.
*/
class CObjectIdFactory
{
public:
	/** The number of bits allocated to store seconds since epoch. */
	static const int SecondsSinceEpochBits = 32;

	/** The number of bits allocated to store the instance identifier. */
	static const int InstanceIdBits = 18;

	/** The number of bits allocated to store the random variant bits. */
	static const int RandomVariantBits = 14;

	/** Identifier for the maximum instance. */
	static const uint32 MaxInstanceId = (1 << InstanceIdBits) - 1;

	/** The maximum random variant. */
	static const uint32 MaxRandomVariant = (1 << RandomVariantBits) - 1;

	/** Magic number to signify an invalid ID. */
	static const ObjectId InvalidId = 0;


	/**
	Constructor.

	\param	instanceId	Identifier for the instance.
	*/
	CObjectIdFactory(uint32 instanceId);


	/**
	Makes a request to the factory to create a new valid ObjectId.

	Only the factory should be allowed to construct these, since it has all the information needed to ensure the key is
	generated correctly.

	\return	The new object identifier.
	*/
	ObjectId CreateObjectId();


	/**
	Gets seconds since epoch.

	\return	The seconds since epoch.
	*/
	uint32 GetSecondsSinceEpoch(ObjectId objectId);


	/**
	Gets instance identifier.

	\return	The instance identifier.
	*/
	uint32 GetInstanceId(ObjectId objectId);


	/**
	Gets random variant.

	\return	The random variant.
	*/
	uint32 GetRandomVariant(ObjectId objectId);

private:
	uint32 m_instanceId;
	uint32 m_randomVariant;
	uint32 m_lastRandomVariantSeed;
	int32 m_secondsSinceEpoch;

	// DO NOT IMPLEMENT.
	CObjectIdFactory();
};
}