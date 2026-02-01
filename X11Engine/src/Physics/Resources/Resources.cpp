#include "Resources.h"

#include <PxPhysicsAPI.h>

#include "Logger.h"
#include "LoggerFactory.h"

static physx::PxDefaultErrorCallback gDefaultErrorCallback;
static physx::PxDefaultAllocator gDefaultAllocatorCallback;

namespace Physics {

constexpr char PVD_HOST[] = "127.0.0.1";

Resources::Resources() {
    auto logger = LoggerFactory::getLogger("Physics");

    foundation = PxCreateFoundation(
        PX_PHYSICS_VERSION, gDefaultAllocatorCallback, gDefaultErrorCallback);
    if (foundation == nullptr) {
        logger.error("PhysX foudation was not initialized");
        throw;
    }

    pvd = PxCreatePvd(*foundation);
    physx::PxPvdTransport* transport =
        physx::PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
    bool is_connected =
        pvd->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
    if (is_connected)
        logger.debug("Connected to PhysX debugger");
    else
        logger.debug("Not connected to PhysX debugger");

    bool record_memory_allocations = true;
    physics = PxCreatePhysics(PX_PHYSICS_VERSION, *foundation,
                              physx::PxTolerancesScale(),
                              record_memory_allocations, pvd);
}

physx::PxPhysics* Resources::getPhysics() { return get().physics; }

Resources& Resources::get() {
    static Resources instance;
    return instance;
}

};  // namespace Physics