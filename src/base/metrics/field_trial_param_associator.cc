// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/metrics/field_trial_param_associator.h"

#include "base/metrics/field_trial.h"

namespace base {

FieldTrialParamAssociator::FieldTrialParamAssociator() {}
FieldTrialParamAssociator::~FieldTrialParamAssociator() {}

// static
FieldTrialParamAssociator* FieldTrialParamAssociator::GetInstance() {
  return Singleton<FieldTrialParamAssociator,
                   LeakySingletonTraits<FieldTrialParamAssociator>>::get();
}

bool FieldTrialParamAssociator::AssociateFieldTrialParams(
    const std::string& trial_name,
    const std::string& group_name,
    const FieldTrialParams& params) {
  AutoLock scoped_lock(lock_);

  if (FieldTrialList::IsTrialActive(trial_name))
    return false;

  const FieldTrialKey key(trial_name, group_name);
  if (ContainsKey(field_trial_params_, key))
    return false;

  field_trial_params_[key] = params;
  return true;
}

bool FieldTrialParamAssociator::GetFieldTrialParams(
    const std::string& trial_name,
    FieldTrialParams* params) {
  AutoLock scoped_lock(lock_);

  const std::string group_name = FieldTrialList::FindFullName(trial_name);
  const FieldTrialKey key(trial_name, group_name);
  if (!ContainsKey(field_trial_params_, key))
    return false;

  *params = field_trial_params_[key];
  return true;
}

void FieldTrialParamAssociator::ClearAllParamsForTesting() {
  AutoLock scoped_lock(lock_);
  field_trial_params_.clear();
}

}  // namespace base