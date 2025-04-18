/*
 * Copyright (c) 2019-2024 Key4hep-Project.
 *
 * This file is part of Key4hep.
 * See https://key4hep.github.io/key4hep-doc/ for further info.
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
#ifndef CELLIDSVC_H
#define CELLIDSVC_H

#include <Gaudi/Property.h>
#include <GaudiKernel/Service.h>

#include <string>

class IGeoSvc;

class TrackingCellIDEncodingSvc : public extends<Service, IService> {
public:
  TrackingCellIDEncodingSvc(const std::string& name, ISvcLocator* svc);

  StatusCode initialize() final;
  StatusCode finalize() final;

  /// Property to configure which DD4hep constant to use for the encodingString
  Gaudi::Property<std::string> m_encodingStringVariable{
      this, "EncodingStringParameterName", "GlobalTrackerReadoutID",
      "The name of the DD4hep constant that contains the Encoding string for tracking detectors"};

  Gaudi::Property<std::string> m_geoSvcName{this, "GeoSvcName", "GeoSvc", "The name of the GeoSvc instance"};

private:
  SmartIF<IGeoSvc> m_geoSvc;
};

#endif // CELLIDSVC_H
