// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "IMovieSceneTrackInstance.h"


class UMovieSceneSlomoTrack;


/**
 * Instance of a UMovieSceneSlomoTrack
 */
class FMovieSceneSlomoTrackInstance
	: public IMovieSceneTrackInstance
{
public:

	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InSlomoTrack The event track to create an instance for.
	 */
	FMovieSceneSlomoTrackInstance(UMovieSceneSlomoTrack& InSlomoTrack);

	/** Virtual destructor. */
	virtual ~FMovieSceneSlomoTrackInstance() { }

public:

	// IMovieSceneTrackInstance interface

	virtual void ClearInstance(IMovieScenePlayer& Player, FMovieSceneSequenceInstance& SequenceInstance) override { }
	virtual void RefreshInstance(const TArray<UObject*>& RuntimeObjects, IMovieScenePlayer& Player, FMovieSceneSequenceInstance& SequenceInstance) override { }
	virtual void RestoreState(const TArray<UObject*>& RuntimeObjects, IMovieScenePlayer& Player, FMovieSceneSequenceInstance& SequenceInstance) override;
	virtual void SaveState(const TArray<UObject*>& RuntimeObjects, IMovieScenePlayer& Player, FMovieSceneSequenceInstance& SequenceInstance) override;
	virtual void Update(float Position, float LastPosition, const TArray<UObject*>& RuntimeObjects, IMovieScenePlayer& Player, FMovieSceneSequenceInstance& SequenceInstance, EMovieSceneUpdatePass UpdatePass) override;

protected:

	/** Get the world settings object. */
	AWorldSettings* GetWorld() const;

	/** Check whether the slow motion should be applied. */
	bool ShouldBeApplied() const;

private:

	/** Track that is being instanced */
	UMovieSceneSlomoTrack* SlomoTrack;

	float InitMatineeTimeDilation;
};
