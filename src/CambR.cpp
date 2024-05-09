#include "CambR.hpp"

CambR::CambR(float min_x, float max_x, float min_y, float max_y, float min_z, float max_z,
 			int min_grid_resolution, int max_grid_resolution,
			float* hash_table,
			vector<vector<float>>& w1_d, vector<vector<float>>& w2_d, vector<vector<float>>& w1_c, vector<vector<float>>& w2_c, vector<vector<float>>& w3_c) : sam_unit_(min_x, max_x, min_y, max_y, min_z, max_z), enc_unit_(min_grid_resolution, max_grid_resolution) {
    
	states_.assign(128, BEF_SAM);
	
	for (unsigned int i=0; i<128; ++i) {
		features_[i] = {
			{
				.ridx = i,
				.bidx = 0,
				.pidx = 0,
				.feature_vector = {0.0}
			}
		};
	}

	enc_unit_.HashTableLoad (hash_table);

	for (unsigned int i = 0; i < 128; ++i) {
        MlpUnit tem_mlpunit(i);
		tem_mlpunit.MlpWeightLoad(w1_d, w2_d, w1_c, w2_c, w3_c);
        mlp_units_.push_back(tem_mlpunit);
    }
}

void CambR::Cycle() {
	this->ClearOutputs();

	for (auto ray : this->in_rays) {
		this->rays_[ray.ridx] = ray;
	}

	// execute Cycle() of each unit
	sam_unit_.Cycle();
	enc_unit_.Cycle();
	for (auto &mlp_unit : mlp_units_ ) {
		mlp_unit.Cycle();
	}

	// Check sam_unit_ output (0 or 8 batches of samples) and change appropriate element of 'states_' from SAM_IN_PROG -> BEF_ENC
	if (!sam_unit_.out_sample_batches.empty()) {
		unsigned int ridx = sam_unit_.out_sample_batches[0][0].ridx;
		states_[ridx%128] = BEF_ENC;
		samples_.at(ridx) = sam_unit_.out_sample_batches;
	}

	// Check enc_unit_ outputs (0 or more batches of features) and change appropriate element of 'states_' from ENC_IN_PROG -> READY.
	// And of course, store the feature batch in the appropriate element of 'features_'.
	if (!enc_unit_.out_features.empty()) {
		for (auto &out_features_same_batch: enc_unit_.out_features) {
			unsigned int ridx = out_features_same_batch[0].ridx;
			states_[ridx%128] = READY;
			features_[ridx%128] = out_features_same_batch;
		}
	}

	// Check idle MLP and feed them features from 'features_'.
	// And change state of 'states_' too.
	for (int i = 0; i < 128; ++i) {
		MlpUnit& mlp_unit = mlp_units_[i];
		vector<Feature> in_features = features_[i];
		unsigned char state = states_[i];

		if (mlp_unit.is_idle() && state == READY) {
			if (mlp_unit.GetHasOutput()) {
				out_pixels.push_back(mlp_unit.out_pixel);
				if (in_features[0].bidx == 0) {
					mlp_unit.in_features = in_features;
					features_[i] = {
						{
							.ridx = in_features[0].ridx,
							.bidx = 1,
							.pidx = 0,
							.feature_vector = {0.0}
						}
					};
					states_[i] = BEF_ENC;
					mlp_unit.ridx = in_features[0].ridx;
				} else {
					features_[i] = {
						{
							.ridx = in_features[0].ridx + 128,
							.bidx = 0,
							.pidx = 0,
							.feature_vector = {0.0}
						}
					};
					states_[i] = BEF_SAM;
					// mlp_unit.ridx = in_features[0].ridx + 1;
					// Above line in probably not needed, because we didn't feed the MLP unit with the batch YET.
					// When the feature is READY, line 71 will set the `ridx` of `mlp_unit`.
				}
			} else {
				mlp_unit.in_features = in_features;
				features_[i] = {
					{
						.ridx = in_features[0].ridx + (in_features[0].bidx == 7),
						.bidx = (unsigned char)((in_features[0].bidx + 1) % 8),
						.pidx = 0,
						.feature_vector = {0.0}
					}
				};
				if (features_[i][0].bidx == 0) {
					states_[i] = BEF_SAM;
				} else {
					states_[i] = BEF_ENC;
				}
			}

		}
	}

	for (int i = 0; i < 128; ++i) {
		if (states_[i] == BEF_SAM) {
			sam_unit_.in_ray = {rays_[features_[i][0].ridx]};
			states_[i] = SAM_IN_PROG;
			break;
		}
	}

	for (int i = 0; i < 128; ++i) {
		if (states_[i] == BEF_ENC) {
			enc_unit_.in_samples = samples_.at(features_[i][0].ridx)[features_[i][0].bidx];
			states_[i] = ENC_IN_PROG;
			break;
		}
	}
	this->ClearInputs();
}
/*
######################################
def Cycle():
	SAM.Cycle()
	ENC.Cycle()
	for MLP in MLPs:
		MLP.Cycle()

	# Check SAM output (0 or 8 batches of samples) and change appropriate element of 'states' from SAM_IN_PROG -> BEF_ENC.
	if (SAM has output):
		ridx = output.ridx
		states[ridx % 128] = BEF_ENC
		samples[ridx] = output

	# Check ENC outputs (0 or more batches of features) and change appropriate element of 'states' from ENC_IN_PROG -> READY.
	# And of course, store the feature batch in the appropriate element of 'features'.
	if (ENC has output):
		ridx = output.ridx
		states[ridx % 128] = READY
		features[ridx % 128] = output

	# Check idle MLP and feed them features from 'features'.
	# And change state of 'states' too.
	for i in range(128):
		MLP = MLPs[i]
		feature = features[i]
		state = states[i]
		if MLP.is_idle() && state == READY:
			if MLP has output:
				out_pixels.append(output)
				if feature.bidx == 0: # Non-early termination.
					MLP.in_features = feature
					features[i] = (empty feature batch with ridx and bidx(=1) of next batch)
					states[i] = BEF_ENC
				else: # Early termination.
					features[i] = (empty feature batch with ridx of next ray and bidx of 0)
					states[i] = BEF_SAM
			else:
				MLP.in_features = feature
				features[i] = (empty feature batch with ridx and bidx of next batch)
				if features[i].bidx == 0: # Next batch will come from new ray.
					states[i] = BEF_SAM # New ray, new sampling!
				else:
					states[i] = BEF_ENC # This ray is already sampled.

	for i in range(128):
		if states[i] == BEF_SAM:
			SAM.in_ray = rays[features[i].ridx]
			states[i] = SAM_IN_PROG
			break

	for i in range(128):
		if states[i] == BEF_ENC:
			ENC.in_samples = samples[features[i].ridx][features[i].bidx]
			states[i] = ENC_IN_PROG
			break

##################################################
*/

void CambR::ClearInputs() {
    this->in_rays.clear();
}

void CambR::ClearOutputs() {
    this->out_pixels.clear();
}